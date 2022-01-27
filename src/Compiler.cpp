#include <Python.h>
#include <marshal.h>

#include "Compiler.h"

#include <fstream>
#include <vector>
#include <sstream>
#include <sys/stat.h>
#include <iostream>

static const unsigned char EOL[4] = "EOL";

inline bool file_exists(const std::string &name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

class Program {
    int line_no{0};
    PyCodeObject *codeObject;
public:
    Program() {
        PyObject *co_cellvars = PyTuple_New(0);
        PyObject *code = PyBytes_FromStringAndSize("", 0);
        PyObject *co_consts = PyTuple_New(0);
        PyObject *co_filename = PyUnicode_FromString("test.py");
        PyObject *co_freevars = PyTuple_New(0);
        PyObject *co_lnotab = PyBytes_FromString("");
        PyObject *co_name = PyUnicode_FromString("<module>");
        PyObject *co_names = PyTuple_New(0);
        PyObject *co_varnames = PyTuple_New(0);
        //PyCodeObject *PyCode_New(int argcount, int kwonlyargcount, int nlocals, int stacksize, int flags, PyObject *code,
        // PyObject *consts, PyObject *names, PyObject *varnames, PyObject *freevars, PyObject *cellvars, PyObject *filename,
        // PyObject *name, int firstlineno, PyObject *lnotab)
        codeObject = PyCode_New(0,
                                0,
                                0,
                                10, // stacksize TODO
                                64,
                                code,
                                co_consts,
                                co_names,
                                co_varnames,
                                co_freevars,
                                co_cellvars,
                                co_filename,
                                co_name,
                                1,
                                co_lnotab);
    }

    void operator+=(const unsigned char *command) {
        PyBytes_ConcatAndDel(&codeObject->co_code, PyBytes_FromString(reinterpret_cast<const char *>(command)));
        line_no += 2;
    }

    void operator+=(const unsigned char command) {
        char t = (char) (command);
        PyBytes_ConcatAndDel(&codeObject->co_code, PyBytes_FromStringAndSize(&t, 1));
        line_no += 2;
    }

    void operator+=(const std::string &command) {
        PyBytes_ConcatAndDel(&codeObject->co_code, PyBytes_FromStringAndSize(command.c_str(),
                                                                             command.size())); // use size to avoid extra null character
        line_no += 2;
    }

    Program &operator<<(const unsigned char command) {
        char t = (char) (command);
        PyBytes_ConcatAndDel(&codeObject->co_code, PyBytes_FromStringAndSize(&t, 1));
        return *this;
    }

    Program &operator<<(const unsigned char *command) {
        if (command == EOL) line_no += 2;
        return *this;
    }


    Program &operator<<(const std::string &command) {
        PyBytes_ConcatAndDel(&codeObject->co_code, PyBytes_FromStringAndSize(command.c_str(),
                                                                             command.size())); // use size to avoid extra null character
        return *this;
    }

    void addLines(int count) {
        line_no += count;
    }

    int getLineNo() const {
        return line_no;
    }

    std::string getCode() const {
        char *buffer;
        Py_ssize_t length;
        if (PyBytes_AsStringAndSize(codeObject->co_code, &buffer, &length) == -1) throw "conversion failed";
        return {buffer, static_cast<unsigned long long>(length)};
    }

    int findValue(PyObject *value) {
        int index = PySequence_Index(codeObject->co_names, value);
        PyErr_Clear();
        return index;
    }

    int findValueOrAdd(PyObject *value) {
        int index = PySequence_Index(codeObject->co_names, value);
        PyErr_Clear();
        if (index == -1) {
            int size_old = PyTuple_Size(codeObject->co_names);
            _PyTuple_Resize(&codeObject->co_names, size_old + 1);
            PyTuple_SetItem(codeObject->co_names, size_old, value);
            index = size_old;
        }
        return index;
    }

    int findConstOrAdd(PyObject *value) {
        int index = PySequence_Index(codeObject->co_consts, value);
        PyErr_Clear();
        if (index == -1) {
            int size_old = PyTuple_Size(codeObject->co_consts);
            _PyTuple_Resize(&codeObject->co_consts, size_old + 1);
            PyTuple_SetItem(codeObject->co_consts, size_old, value);
            index = size_old;
        }
        return index;
    }

    PyCodeObject *getCodeObject() {
        int index = PySequence_Index(codeObject->co_consts, Py_None);
        PyErr_Clear();
        if (index == -1) {
            int size_old = PyTuple_Size(codeObject->co_consts);
            _PyTuple_Resize(&codeObject->co_consts, size_old + 1);
            PyTuple_SetItem(codeObject->co_consts, size_old, Py_None);
            index = size_old;
        }
        (*this) << (unsigned char) LOAD_CONST << (unsigned char) index << EOL;
        (*this) << (unsigned char) RETURN_VALUE << '\0' << EOL;
        return codeObject;
    }
};

void compileNode(typename AST::AbstractSyntaxTree<Token *>::Const_Iterator &node, Program &program);


void constructWhile(typename AST::AbstractSyntaxTree<Token *>::Const_Iterator &node, Program &program) {
    ++node; // first argument (operator)
    cmp_op oper;
    if (node->getData()->value == "==") oper = EQ;
    else if (node->getData()->value == "!=") oper = NEQ;
    else if (node->getData()->value == "<") oper = LT;
    else if (node->getData()->value == "<=") oper = LTE;
    else if (node->getData()->value == ">") oper = GT;
    else if (node->getData()->value == ">=") oper = GTE;
    ++node; // first argument(identifier)
    std::string name = node->getData()->value;
    PyObject *py_name = PyUnicode_FromStringAndSize(name.c_str(), name.size());
    int name_index = program.findValueOrAdd(py_name);

    ++node; // second argument(const)
    int const_index;
    try {
        const_index = program.findConstOrAdd(PyLong_FromLong(std::stoi(node->getData()->value)));
    } catch (std::invalid_argument& e){
        throw CompilationError("Can't compare with a non constant! Error at line: " + std::to_string(node->getLineNo()));
    }

    int begin = program.getLineNo() + 2;

    program << LOAD_NAME << (unsigned char) name_index << EOL;
    program << LOAD_CONST << (unsigned char) const_index << EOL;
    program << COMPARE_OP << oper << EOL;
    ++node; // body
    if (node->getData()->type == endwhile_) {
        std::stringstream error_msg;
        error_msg << "Error on line: " << node->getLineNo() << " ,can't have a while loop with an empty body!" << std::endl;
        throw CompilationError(error_msg.str());
    }
    Program temp;
    AST::AbstractSyntaxTree<Token *>::Const_Iterator node_cpy = node;
    do {
        compileNode(node_cpy, temp);
        ++node_cpy;
    } while (node_cpy->getData()->type != endwhile_);
    program << POP_JUMP_IF_FALSE << (unsigned char) (program.getLineNo() + temp.getLineNo() + 6)
            << EOL; // +2 for current op and +2 for jump absolute
    do {
        compileNode(node, program);
        ++node;
    } while (node->getData()->type != endwhile_);
    program << JUMP_ABSOLUTE << (unsigned char) begin << EOL;
}


void constructIncr(typename AST::AbstractSyntaxTree<Token *>::Const_Iterator &node, Program &program) {
    ++node; // first argument (identifier)
    std::string name = node->getData()->value;
    int name_index = program.findValueOrAdd(PyUnicode_FromStringAndSize(name.c_str(), name.size()));
    int const_index = program.findConstOrAdd(PyLong_FromLong(1));

    program << LOAD_NAME << (unsigned char) name_index << EOL;
    program << LOAD_CONST << (unsigned char) const_index << EOL;
    program << INPLACE_ADD << (unsigned char) 0 << EOL;
    program << STORE_NAME << (unsigned char) name_index << EOL;
}


void constructDecr(typename AST::AbstractSyntaxTree<Token *>::Const_Iterator &node, Program &program) {
    ++node; // first argument (identifier)
    std::string name = node->getData()->value;
    int name_index = program.findValueOrAdd(PyUnicode_FromStringAndSize(name.c_str(), name.size()));
    int const_index = program.findConstOrAdd(PyLong_FromLong(1));

    program << LOAD_NAME << (unsigned char) name_index << EOL;
    program << LOAD_CONST << (unsigned char) const_index << EOL;
    program << INPLACE_SUBTRACT << (unsigned char) 0 << EOL;
    program << STORE_NAME << (unsigned char) name_index << EOL;
    // add runtime check for negative numbers, if negative add number by 2**32
    const_index = program.findConstOrAdd(PyLong_FromLong(0));
    program << LOAD_NAME << (unsigned char) name_index << EOL;
    program << LOAD_CONST << (unsigned char) const_index << EOL;
    program << COMPARE_OP << (unsigned char) LT << EOL;
    int current_lineno = program.getLineNo();
    program << POP_JUMP_IF_FALSE << (unsigned char) current_lineno + 12 << EOL;
    program << LOAD_NAME << (unsigned char) name_index << EOL;
    const_index = program.findConstOrAdd(PyLong_FromLongLong(4294967296));
    program << LOAD_CONST << (unsigned char) const_index << EOL;
    program << INPLACE_ADD << (unsigned char) 0 << EOL;
    program << STORE_NAME << (unsigned char) name_index << EOL;
}


void constructStore(typename AST::AbstractSyntaxTree<Token *>::Const_Iterator &node, Program &program) {
    ++node; // first argument (identifier)
    std::string name = node->getData()->value;
    int var_index = program.findValueOrAdd(PyUnicode_FromStringAndSize(name.c_str(), name.size()));

    ++node; // second argument (const value)
    unsigned int value = std::stoi(node->getData()->value);
    int const_index = program.findConstOrAdd(PyLong_FromLong(value));

    program << LOAD_CONST << (unsigned char) const_index << EOL;
    program << STORE_NAME << (unsigned char) var_index << EOL;
}


void constructPrint(typename AST::AbstractSyntaxTree<Token *>::Const_Iterator &node, Program &program) {
    ++node; // first argument (variable)
    int print_index = program.findValueOrAdd(PyUnicode_FromStringAndSize("print", 5));

    std::string name = node->getData()->value;
    int var_index = program.findValueOrAdd(PyUnicode_FromStringAndSize(name.c_str(), name.size()));

    program << LOAD_NAME << (unsigned char) print_index << EOL;
    program << LOAD_NAME << (unsigned char) var_index << EOL;
    program << CALL_FUNCTION << (unsigned char) 1 << EOL;
    program << POP_TOP << (unsigned char) 0 << EOL;
}


void constructFunc(typename AST::AbstractSyntaxTree<Token *>::Const_Iterator &node, Program &program) {
    bool by_ref = std::stoi(node->getData()->value);
    ++node; // first argument (module name/identifier)
    std::string module_name = node->getData()->value;

    int sys = program.findValue(PyUnicode_FromStringAndSize("sys", 3));
    assert(sys != -1);

    ++node; // second argument (list of parameters)

    std::vector<std::string> args;
    do {
        if (node->getData()->type != identifier_ && node->getData()->type != endfunc_) {
            std::cout << getTypeString(node->getData()->type) << std::endl;
            std::stringstream error_msg;
            error_msg << "Error on line: " << node->getLineNo() << " ,invalid function argument!" << std::endl;
            throw CompilationError(error_msg.str());
        }
        args.emplace_back(node->getData()->value);
        ++node;
    } while (node->getData()->type != endfunc_);
    { // put the arguments in sys.argv
        for (const std::string &arg: args) {
            int name_i = program.findValue(PyUnicode_FromStringAndSize(arg.c_str(), arg.size()));
            program << LOAD_NAME << (unsigned char) name_i << EOL;
        }
        int argv = program.findValueOrAdd(PyUnicode_FromStringAndSize("argv", 4));
        program << BUILD_LIST << (unsigned char) args.size() << EOL;
        program << LOAD_NAME << (unsigned char) sys << EOL;
        program << STORE_ATTR << (unsigned char) argv << EOL;
    }

    int module = program.findValue(PyUnicode_FromStringAndSize(module_name.c_str(), module_name.size()));
    if (module == -1) {
        if (!file_exists("../" + module_name + ".pyc")) {
            std::stringstream error_msg;
            error_msg << "Error on line: " << node->getLineNo() << " ,file not found!" << std::endl;
            throw CompilationError(error_msg.str());
        }
        module = program.findValueOrAdd(PyUnicode_FromStringAndSize(module_name.c_str(), module_name.size()));
        int zero = program.findConstOrAdd(PyLong_FromLong(0));
        int none = program.findConstOrAdd(Py_None);
        program << LOAD_CONST << (unsigned char) zero << EOL;
        program << LOAD_CONST << (unsigned char) none << EOL;
        program << IMPORT_NAME << (unsigned char) module << EOL;
        program << STORE_NAME << (unsigned char) module << EOL;
    } else {
        int import_lib = program.findValueOrAdd(PyUnicode_FromStringAndSize("importlib", 9));
        int reload = program.findValueOrAdd(PyUnicode_FromStringAndSize("reload", 6));
        program << LOAD_NAME << (unsigned char) import_lib << EOL;
        program << LOAD_METHOD << (unsigned char) reload << EOL;
        program << LOAD_NAME << (unsigned char) module << EOL;
        program << CALL_METHOD << (unsigned char) 1 << EOL;
        program << POP_TOP << (unsigned char) 0 << EOL;
    }


    //unpack output
    if (by_ref) {
        int i = 1;
        for (const std::string &name: args) {
            int name_i = program.findValue(PyUnicode_FromStringAndSize(name.c_str(), name.size()));
            std::string output_name = "z" + std::to_string(i);
            int output = program.findValueOrAdd(PyUnicode_FromStringAndSize(output_name.c_str(), output_name.size()));
            program << LOAD_NAME << (unsigned char) module << EOL;
            program << LOAD_ATTR << (unsigned char) output << EOL;
            program << STORE_NAME << (unsigned char) name_i << EOL;
        }
    } else {
        int attr_error = program.findValueOrAdd(PyUnicode_FromStringAndSize("AttributeError", 14));

        program << SETUP_FINALLY << (unsigned char) 10 << EOL;
        int start = program.getLineNo();
        int i = 1;
        for (const std::string &name: args) {
            int name_i = program.findValue(PyUnicode_FromStringAndSize(name.c_str(), name.size()));
            std::string output_name = "z" + std::to_string(i);
            int output = program.findValueOrAdd(PyUnicode_FromStringAndSize(output_name.c_str(), output_name.size()));
            program << LOAD_NAME << (unsigned char) module << EOL;
            program << LOAD_ATTR << (unsigned char) output << EOL;
            program << STORE_NAME << (unsigned char) output << EOL;
        }
        program << POP_BLOCK << (unsigned char) 0 << EOL;
        program << JUMP_FORWARD << (unsigned char) 18 << EOL;

        program << DUP_TOP << (unsigned char) 0 << EOL;
        program << LOAD_NAME << (unsigned char) attr_error << EOL;
        int jump = program.getLineNo() + 18;
        program << JUMP_IF_NOT_EXC_MATCH << (unsigned int) jump << EOL;
        program << POP_TOP << (unsigned char) 0 << EOL;
        program << POP_TOP << (unsigned char) 0 << EOL;
        program << POP_TOP << (unsigned char) 0 << EOL;

        program << POP_EXCEPT << (unsigned char) 0 << EOL;
        jump = program.getLineNo() + 12;
        program << JUMP_FORWARD << (unsigned char) 2 << EOL;
        program << RERAISE << (unsigned char) 0 << EOL;
    }
}


void compileNode(typename AST::AbstractSyntaxTree<Token *>::Const_Iterator &node, Program &program) {
    switch (node->getData()->type) {
        case root_:
        {
            int zero = program.findConstOrAdd(PyLong_FromLong(0));
            int none = program.findConstOrAdd(Py_None);
            int sys = program.findValueOrAdd(PyUnicode_FromStringAndSize("sys", 3));
            int enumerate = program.findValueOrAdd(PyUnicode_FromStringAndSize("enumerate", 9));
            int argv = program.findValueOrAdd(PyUnicode_FromStringAndSize("argv", 4));
            int i = program.findValueOrAdd(PyUnicode_FromStringAndSize("i", 1));
            int x = program.findConstOrAdd(PyUnicode_FromStringAndSize("x", 1));
            int v = program.findValueOrAdd(PyUnicode_FromStringAndSize("v", 1));
            int int_ = program.findValueOrAdd(PyUnicode_FromStringAndSize("int", 3));
            int gloabls = program.findValueOrAdd(PyUnicode_FromStringAndSize("globals", 7));
            int value_error = program.findValueOrAdd(PyUnicode_FromStringAndSize("ValueError", 10));
            program << LOAD_CONST << (unsigned char) zero << EOL;
            program << LOAD_CONST << (unsigned char) none << EOL;
            program << IMPORT_NAME << (unsigned char) sys << EOL;
            program << STORE_NAME << (unsigned char) sys << EOL;

            program << LOAD_NAME << (unsigned char) enumerate << EOL;
            program << LOAD_NAME << (unsigned char) sys << EOL;
            program << LOAD_ATTR << (unsigned char) argv << EOL;
            program << CALL_FUNCTION << (unsigned char) 1 << EOL;
            program << GET_ITER << (unsigned char) 0 << EOL;
            program << FOR_ITER << (unsigned char) 52;
            int start = program.getLineNo();
            program << UNPACK_SEQUENCE << (unsigned char) 2 << EOL;
            program << STORE_NAME << (unsigned char) i << EOL;
            program << STORE_NAME << (unsigned char) v << EOL;

            program << SETUP_FINALLY << (unsigned char) 24 << EOL;

            program << LOAD_NAME << (unsigned char) int_ << EOL;
            program << LOAD_NAME << (unsigned char) v << EOL;
            program << CALL_FUNCTION << (unsigned char) 1 << EOL;
            program << LOAD_NAME << (unsigned char) gloabls << EOL;
            program << CALL_FUNCTION << (unsigned char) 0 << EOL;
            program << LOAD_CONST << (unsigned char) x << EOL;
            program << LOAD_NAME << (unsigned char) i << EOL;
            program << FORMAT_VALUE << (unsigned char) 0 << EOL;
            program << BUILD_STRING << (unsigned char) 2 << EOL;
            program << STORE_SUBSCR << (unsigned char) 0 << EOL;
            program << POP_BLOCK << (unsigned char) 0 << EOL;
            program << JUMP_ABSOLUTE << (unsigned int) start << EOL;

            program << DUP_TOP << (unsigned char) 0 << EOL;
            program << LOAD_NAME << (unsigned char) value_error << EOL;
            program << JUMP_IF_NOT_EXC_MATCH << (unsigned int) 68 << EOL;
            program << POP_TOP << (unsigned char) 0 << EOL;
            program << POP_TOP << (unsigned char) 0 << EOL;
            program << POP_TOP << (unsigned char) 0 << EOL;

            program << POP_EXCEPT << (unsigned char) 0 << EOL;
            program << JUMP_ABSOLUTE << (unsigned int) start << EOL;
            program << RERAISE << (unsigned char) 0 << EOL;
            program << JUMP_ABSOLUTE << (unsigned int) start << EOL;
        }
            break;
        case while_:
            constructWhile(node, program);
            break;

        case incr_:
            constructIncr(node, program);
            break;

        case decr_:
            constructDecr(node, program);
            break;

        case identifier_:
        case const_:
            std::cerr << "Possible error" << std::endl;
            break;

        case operator_:
            assert(node->getData()->value == "=");
            constructStore(node, program);
            break;

        case print_:
            constructPrint(node, program);
            break;

        case func_:
            constructFunc(node, program);

        default:
            return;
    }
}


void compile(const AST::AbstractSyntaxTree<Token *> &ast, const std::string& filename) {
    Py_SetPythonHome(L"C:/Users/nibor/AppData/Local/Programs/Python/Python39");
    Py_Initialize();
    Program program;
    for (auto node = ast.begin(); node != ast.end(); ++node) {
        compileNode(node, program);
    }

    char *marshalled;
    Py_ssize_t length;
    PyCodeObject *codeObject = program.getCodeObject();
    PyBytes_AsStringAndSize(codeObject->co_code, &marshalled, &length);
    PyObject *source = PyMarshal_WriteObjectToString((PyObject *) (codeObject), Py_MARSHAL_VERSION);
    PyBytes_AsStringAndSize(source, &marshalled, &length);
    std::ofstream file("../" + filename + ".pyc", std::ios::binary | std::ios::trunc);
    {
        constexpr short int magic_int = (PY_MINOR_VERSION == 9 ? 3425 : 3400); // magic int for python 3.8 or 3.9
        char byte1, byte2, byte3 = '\r', byte4 = '\n';
        byte2 = (char) (magic_int >> 8);
        byte1 = (char) (magic_int & 255);
        file << byte1 << byte2 << byte3 << byte4 << '\x00' << '\x00' << '\x00' << '\x00';
    }
    {
        int t = time(nullptr); // timestamp
        unsigned char timestamp[4];
        for (int i = 0; i < 4; i++)
            timestamp[i] = (t >> (i * 8));

        for (unsigned char c: timestamp) {
            file << c;
        }
    }
    {
        int obj_size = 20;
        unsigned char size[4];
        for (int i = 0; i < 4; i++)
            size[i] = (obj_size >> (i * 8));

        for (unsigned char c: size) {
            file << c;
        }
    }
    for (int i = 0; i <= length; i++) {
        file << (unsigned char)marshalled[i];
    }
    file.flush();
    Py_Finalize();
}
