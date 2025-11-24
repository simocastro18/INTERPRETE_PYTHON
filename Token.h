#if !defined(TOKEN_H)
#define TOKEN_H

#include <string>

struct Token {

    static constexpr int LP    =  0; // (
    static constexpr int RP    =  1; // )
    static constexpr int EQ    =  2; // =
    static constexpr int ADD   =  3; // +
    static constexpr int SUB   =  4; // -
    static constexpr int MUL   =  5; // *
    static constexpr int DIV   =  6; // /
    static constexpr int ID    =  7; // identifier
    static constexpr int CONST =  8; // integer literal
    static constexpr int LT      =  9;  // <
    static constexpr int GT      = 10;  // >
    static constexpr int LE      = 11;  // <=
    static constexpr int GE      = 12;  // >=
    static constexpr int EQEQ    = 13;  // ==
    static constexpr int NEQ     = 14;  // !=
    static constexpr int FLOORDIV= 15;  // //
    static constexpr int COLON   = 16;  // :
    static constexpr int COMMA   = 17;  // ,
    static constexpr int WHILE   = 18;  // while
    static constexpr int NOT     = 19;  // not
    static constexpr int AND     = 20;  // and
    static constexpr int OR      = 21;  // or
    static constexpr int PRINT   = 22;  // print
    static constexpr int TRUE    = 23;  // True
    static constexpr int FALSE   = 24;  // False
    static constexpr int DOT     = 25;  // .
    static constexpr int MOD     = 26;  // %
    static constexpr int NEWLINE = 27;
    static constexpr int INDENT  = 28;
    static constexpr int DEDENT  = 29;
    static constexpr int ENDMARKER=30;
    static constexpr int IF      = 31;  // if
    static constexpr int ELSE    = 32;  // else
    static constexpr int LBRACK  = 33;  // [
    static constexpr int RBRACK  = 34;  // ]
    static constexpr int ELIF    = 35;
    static constexpr int BREAK   = 36;  // break

    static constexpr const char* tag2string[]{
        "LP","RP","EQ","OP","OP","OP","OP","ID","CONST",
        "LT","GT","LE","GE","EQEQ","NEQ","FLOORDIV","COLON","COMMA",
        "WHILE","NOT","AND","OR","PRINT","TRUE","FALSE",
        "DOT","MOD","NEWLINE","INDENT","DEDENT","ENDMARKER",
        "IF","ELSE","LBRACK","RBRACK","ELIF","BREAK"
    };

    Token(int t, const char* w) : tag{ t }, word{ w } {}
    Token(int t, std::string w) : tag{ t }, word{ std::move(w) } {}

    int tag;
    std::string word;
};

std::ostream& operator<<(std::ostream& os, const Token& t);

#endif
