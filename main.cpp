#include "src/Compiler.h"
#include <iostream>

int main(){

    Py_SetPythonHome(L"C:/Users/nibor/AppData/Local/Programs/Python/Python39");
    Py_Initialize();

    /*{
        char *m;
        Py_ssize_t l;
        PyObject *code1 = Py_CompileString("x=1", "test.py", Py_file_input);
        PyObject *source = PyMarshal_WriteObjectToString((PyObject *) (code1), Py_MARSHAL_VERSION);
        PyBytes_AsStringAndSize(source, &m, &l);
        std::ofstream file("../test.marsh", std::ios::binary | std::ios::trunc);
        for (int i = 0; i <= l; i++) {
            file << m[i];
        }
        file.flush();
        file.close();
    }*/

    /*PyObject* co_cellvars = PyTuple_New(0);
    PyObject* code = PyBytes_FromString("e\x00d\x00\x83\x01\x01\x00d\x01S\x00");
    PyObject* code1 = Py_CompileString("x=1", "test.py", Py_file_input);
    char *m;
    Py_ssize_t l;
    PyBytes_AsStringAndSize(PyObject_GetAttr(code1, PyUnicode_FromString("co_code")), &m, &l);
    for (int i = 0; i < l;i++)
        std::cout << (int)m[i] << " ";
    PyObject* co_consts = PyTuple_New(2);
    {
        PyObject* string = PyUnicode_FromString("Hello World");
        PyTuple_SetItem(co_consts, 0, string);
        PyTuple_SetItem(co_consts, 1, Py_None);
    }
    PyObject* co_filename = PyUnicode_FromString("test.pyc");
    PyObject* co_freevars = PyTuple_New(0);
    PyObject* co_lnotab = PyBytes_FromString("");
    PyObject* co_name = PyUnicode_FromString("<module>");
    PyObject* co_names = PyTuple_New(1);
    {
        PyObject* string = PyUnicode_FromString("print");
        PyTuple_SetItem(co_names, 0, string);
    }
    PyObject* co_varnames = PyTuple_New(0);
    //PyCodeObject *PyCode_New(int argcount, int kwonlyargcount, int nlocals, int stacksize, int flags, PyObject *code,
    // PyObject *consts, PyObject *names, PyObject *varnames, PyObject *freevars, PyObject *cellvars, PyObject *filename,
    // PyObject *name, int firstlineno, PyObject *lnotab)
    PyCodeObject *pyCodeObject = PyCode_New(0,
                                            0,
                                            0,
                                            2,
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
    // from python source
    // Nothing is actually declared to be a PyObject, but every pointer to
    // a Python object can be cast to a PyObject*.  This is inheritance built
    // by hand.  Similarly every pointer to a variable-size Python object can,
    // in addition, be cast to PyVarObject*.

    char *marshalled;
    Py_ssize_t length;
    PyObject* source = PyMarshal_WriteObjectToString((PyObject *)(code1), Py_MARSHAL_VERSION);
    int source_str = PyBytes_AsStringAndSize(source, &marshalled, &length);
    std::cout << source_str  << '\n';

    std::ofstream file("../test_ref.pyc", std::ios::binary | std::ios::trunc);
    short int magic_int = 3425; // magic int for python 3.9
    char byte1, byte2, byte3 = '\r', byte4 = '\n';
    {
        byte2 = (char) (magic_int >> 8);
        byte1 = (char) (magic_int & 255);
        file << byte1 << byte2 << byte3 << byte4 << '\x00' << '\x00' << '\x00' << '\x00';
    }
    {
        int t = 1638344492; // timestamp
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
        file << marshalled[i];
    }
    file.flush();

    Py_Finalize();*/


    return 0;
}
