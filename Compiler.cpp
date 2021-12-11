
#include "Compiler.h"
#include <functional>

enum node_type {
    while_, incr_, decr_, identifier_, store_, print_
};

enum opcodes {
    POP_TOP = 1,
    ROT_TWO = 2,
    ROT_THREE = 3,
    DUP_TOP = 4,
    DUP_TOP_TWO = 5,
    ROT_FOUR = 6,
    NOP = 9,
    UNARY_POSITIVE = 10,
    UNARY_NEGATIVE = 11,
    UNARY_NOT = 12,
    UNARY_INVERT = 15,
    BINARY_MATRIX_MULTIPLY = 16,
    INPLACE_MATRIX_MULTIPLY = 17,
    BINARY_POWER = 19,
    BINARY_MULTIPLY = 20,
    BINARY_MODULO = 22,
    BINARY_ADD = 23,
    BINARY_SUBTRACT = 24,
    BINARY_SUBSCR = 25,
    BINARY_FLOOR_DIVIDE = 26,
    BINARY_TRUE_DIVIDE = 27,
    INPLACE_FLOOR_DIVIDE = 28,
    INPLACE_TRUE_DIVIDE = 29,
    RERAISE = 48,
    WITH_EXCEPT_START = 49,
    GET_AITER = 50,
    GET_ANEXT = 51,
    BEFORE_ASYNC_WITH = 52,
    END_ASYNC_FOR = 54,
    INPLACE_ADD = 55,
    INPLACE_SUBTRACT = 56,
    INPLACE_MULTIPLY = 57,
    INPLACE_MODULO = 59,
    STORE_SUBSCR = 60,
    DELETE_SUBSCR = 61,
    BINARY_LSHIFT = 62,
    BINARY_RSHIFT = 63,
    BINARY_AND = 64,
    BINARY_XOR = 65,
    BINARY_OR = 66,
    INPLACE_POWER = 67,
    GET_ITER = 68,
    GET_YIELD_FROM_ITER = 69,
    PRINT_EXPR = 70,
    LOAD_BUILD_CLASS = 71,
    YIELD_FROM = 72,
    GET_AWAITABLE = 73,
    LOAD_ASSERTION_ERROR = 74,
    INPLACE_LSHIFT = 75,
    INPLACE_RSHIFT = 76,
    INPLACE_AND = 77,
    INPLACE_XOR = 78,
    INPLACE_OR = 79,
    LIST_TO_TUPLE = 82,
    RETURN_VALUE = 83,
    IMPORT_STAR = 84,
    SETUP_ANNOTATIONS = 85,
    YIELD_VALUE = 86,
    POP_BLOCK = 87,
    POP_EXCEPT = 89,
    STORE_NAME = 90,
    DELETE_NAME = 91,
    UNPACK_SEQUENCE = 92,
    FOR_ITER = 93,
    UNPACK_EX = 94,
    STORE_ATTR = 95,
    DELETE_ATTR = 96,
    STORE_GLOBAL = 97,
    DELETE_GLOBAL = 98,
    LOAD_CONST = 100,
    LOAD_NAME = 101,
    BUILD_TUPLE = 102,
    BUILD_LIST = 103,
    BUILD_SET = 104,
    BUILD_MAP = 105,
    LOAD_ATTR = 106,
    COMPARE_OP = 107,
    IMPORT_NAME = 108,
    IMPORT_FROM = 109,
    JUMP_FORWARD = 110,
    JUMP_IF_FALSE_OR_POP = 111,
    JUMP_IF_TRUE_OR_POP = 112,
    JUMP_ABSOLUTE = 113,
    POP_JUMP_IF_FALSE = 114,
    POP_JUMP_IF_TRUE = 115,
    LOAD_GLOBAL = 116,
    IS_OP = 117,
    CONTAINS_OP = 118,
    JUMP_IF_NOT_EXC_MATCH = 121,
    SETUP_FINALLY = 122,
    LOAD_FAST = 124,
    STORE_FAST = 125,
    DELETE_FAST = 126,
    RAISE_VARARGS = 130,
    CALL_FUNCTION = 131,
    MAKE_FUNCTION = 132,
    BUILD_SLICE = 133,
    LOAD_CLOSURE = 135,
    LOAD_DEREF = 136,
    STORE_DEREF = 137,
    DELETE_DEREF = 138,
    CALL_FUNCTION_KW = 141,
    CALL_FUNCTION_EX = 142,
    SETUP_WITH = 143,
    LIST_APPEND = 145,
    SET_ADD = 146,
    MAP_ADD = 147,
    LOAD_CLASSDEREF = 148,
    EXTENDED_ARG = 144,
    SETUP_ASYNC_WITH = 154,
    FORMAT_VALUE = 155,
    BUILD_CONST_KEY_MAP = 156,
    BUILD_STRING = 157,
    LOAD_METHOD = 160,
    CALL_METHOD = 161,
    LIST_EXTEND = 162,
    SET_UPDATE = 163,
    DICT_MERGE = 164,
    DICT_UPDATE = 165
};

enum cmp_op {LT, LTE, EQ, NEQ, GT, GTE};

static char EOL[4] = "EOL";

class Program{
    std::string code;
    int line_no{0};
public:
    template<typename AString>
    void operator+=(const AString& command){
        code += command;
        line_no += 2;
    }

    template<typename AString>
    Program& operator<<(const AString& command){
        if (command == EOL) line_no += 2;
        else code += command;
        return *this;
    }

    void addLines(int count){
        line_no += count;
    }

    int getLineNo() const {
        return line_no;
    }

    const std::string &getCode() const {
        return code;
    }
};

template<typename T>
void constructWhile(typename AST::AbstractSyntaxTree<T>::Iterator& node, Program& program) {
    ++node; // first argument (variable)
    T data = node->getData();
    int begin = program.getLineNo();
    program << (unsigned char) LOAD_NAME << data.identifier << EOL; // we need to give the index to the variables tuple TODO
    program << (unsigned char) LOAD_CONST << '\0' << EOL; // we need to give the index to the consts tuple TODO
    program << (unsigned char) COMPARE_OP << (unsigned char) NEQ << EOL;
    // we first need to compile the body, so we know how far we need to jump
    std::string code;
    int lines;
    std::tie (code, lines) = compileNode(node, program);
    program << (unsigned char) POP_JUMP_IF_FALSE << (unsigned char) program.getLineNo() + lines + 4 << EOL; // +2 for current op and +2 for jump absolute
    program << code;
    program.addLines(lines);
    program << (unsigned char) JUMP_ABSOLUTE << (unsigned char) begin << EOL;
}

template<typename T>
void constructIncr(typename AST::AbstractSyntaxTree<T>::Iterator& node, Program& program) {
    ++node; // first argument (variable)
    T data = node->getData();
    program << (unsigned char) LOAD_NAME << data.identifier << EOL; // we need to give the index to the variables tuple TODO
    program << (unsigned char) LOAD_CONST << '\1'/*index for const list TODO*/ << EOL;
    program += (unsigned char) INPLACE_ADD;
    program << (unsigned char) STORE_NAME << data.identifier << EOL;
}

template<typename T>
void constructDecr(typename AST::AbstractSyntaxTree<T>::Iterator& node, Program& program) {
    ++node; // first argument (variable)
    T data = node->getData();
    program << (unsigned char) LOAD_NAME << data.identifier << EOL; // we need to give the index to the variables tuple TODO
    program << (unsigned char) LOAD_CONST << '\1'/*index for const list TODO*/ << EOL;
    program += (unsigned char) INPLACE_SUBTRACT;
    program << (unsigned char) STORE_NAME << data.identifier << EOL;
}

template<typename T>
void constructStore(typename AST::AbstractSyntaxTree<T>::Iterator& node, Program& program) {
    ++node; // first argument (variable)
    T data = node->getData();
    program << (unsigned char) LOAD_CONST << 'x'/*index from const list TODO*/ << EOL;
    program << (unsigned char) STORE_NAME << data.identifier << EOL;
}

template<typename T>
void compileNode(typename AST::AbstractSyntaxTree<T>::Iterator& node, Program& program){
    switch (node) {
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
            // possible error
            break;

        case store_:
            constructStore(node, program);
            break;

        default:
            return;
    }
}

template<typename T>
std::tuple<std::string, int> compile(const AST::AbstractSyntaxTree<T> &ast) {
    Program program;
    for (auto node = ast.begin(); node != ast.end(); ++node) {
        compileNode(node, program);
    }
    return {program.getCode(), program.getLineNo()};
}



