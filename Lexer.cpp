#include "Lexer.h"     
#include <cctype>         

//analizza caratteri di input e costruisce la lista di token

void Lexer::tokenizeInputFile(std::ifstream& inputFile, std::vector<Token>& inputTokens) {
    auto getch = [&]() -> int { return inputFile.get(); };
    auto peek  = [&]() -> int { return inputFile.peek(); };

    unsigned int row = 1;
    bool at_line_start = true;
    std::vector<int> S = {0};
    int paren = 0;
    bool expect_indent = false;

    int ch = getch();

    auto consume_to_eol = [&](){ while (ch != EOF && ch != '\n') ch = getch(); };
    constexpr int  TAB_WIDTH  = 4;

    auto read_indent = [&](int& c)->int {
        int n = 0;
        while (c == ' ' || c == '\t') {
            if (c == ' ') { 
                ++n; 
                c = getch(); 
            }else { // '\t'
                n += TAB_WIDTH;
                c = getch();
            }
        }
        return n;
    };

    while( ch != EOF) {
        if (ch == EOF) break;

        if (ch == '\n') {
            ++row;
            ch = getch();
            if (paren == 0) {
                inputTokens.emplace_back(Token::NEWLINE, "\\n");
                at_line_start = true;
            }
            continue;
        }

        if (at_line_start) {
            if (paren == 0) {
                int nt = read_indent(ch);
                if (ch == '\n' || ch == '#') {
                    if (ch == '#') consume_to_eol();
                    at_line_start = false;  
                    continue;
                }

                int top = S.back();

                if (expect_indent && nt <= top) {
                    throw SyntaxError(" unexpected indentation", row);
                }

                if (nt > top) {
                    if (!expect_indent){
                        throw SyntaxError(" unexpected indentation", row);
                    }
                    S.push_back(nt);
                    inputTokens.emplace_back(Token::INDENT, "INDENT");
                } else if (nt < top) {
                    while (S.back() > nt) {
                        S.pop_back();
                        inputTokens.emplace_back(Token::DEDENT, "DEDENT");
                    }
                    if (S.back() != nt) {
                        throw SyntaxError(" unexpected indentation", row);
                    }
                }
                expect_indent = false;
                at_line_start = false;
            } else {
                at_line_start = false; 
                while (ch == ' ' || ch == '\t') ch = getch();
                if (ch == '#') { consume_to_eol(); }
            }
        }
        
        if (!at_line_start && (ch == ' ' || ch == '\t')) { 
            ch = getch(); 
            continue; 
        }

        if (ch == '#'){ 
            consume_to_eol(); 
            continue; 
        }

        if (ch == '('){ 
            ++paren; 
            inputTokens.emplace_back(Token::LP, "("); 
            ch = getch(); 
            continue; 
        }

        if (ch == ')'){ 
            if (paren>0){ 
                --paren; 
            }
            inputTokens.emplace_back(Token::RP, ")"); 
            ch = getch(); 
            continue; 
        }
        
        if (ch == '['){ 
            ++paren; 
            inputTokens.emplace_back(Token::LBRACK, "["); 
            ch = getch(); 
            continue; 
        }

        if (ch == ']'){ 
            if (paren>0){
                --paren;
            } 
            inputTokens.emplace_back(Token::RBRACK, "]"); 
            ch = getch(); 
            continue; 
        }

        if (ch == '{'){ 
            ++paren; 
            ch = getch(); 
            continue; 
        }
        if (ch == '}'){ 
            if (paren>0) {
                --paren;
            } 
            ch = getch(); 
            continue; 
        }

        if (ch == ':'){
            inputTokens.emplace_back(Token::COLON, ":");
            ch = getch();
            while (ch == ' ' || ch == '\t') ch = getch();   
            expect_indent = (ch == '\n' || ch == '#');    
            continue;
        }

        if (std::isalpha((unsigned char)ch) || ch == '_'){
            std::string buf;
            do { 
                buf.push_back((char)ch); 
                ch = getch();
            }while (std::isalnum((unsigned char)ch) || ch == '_');

        int tag = Token::ID;
        if(buf == "while"){
            tag = Token::WHILE;
        }else if (buf == "print"){
            tag = Token::PRINT;
        }else if (buf == "and"){
            tag = Token::AND;
        }else if (buf == "or"){
            tag = Token::OR;
        }else if (buf == "not"){
            tag = Token::NOT;
        }else if (buf == "True"){
            tag = Token::TRUE;
        }else if (buf == "False"){
            tag = Token::FALSE;
        }else if (buf == "if"){
            tag = Token::IF;
        }else if (buf == "else"){
            tag = Token::ELSE;
        }else if (buf == "elif"){
            tag = Token::ELIF;
        }else if (buf == "break") {
            tag = Token::BREAK;
        }
        inputTokens.emplace_back(tag, buf);
        continue;
        }

        if (std::isdigit((unsigned char)ch)) {
            std::string buf;
            do { 
                buf.push_back((char)ch); ch = getch(); 
            }while (std::isdigit((unsigned char)ch));
            inputTokens.emplace_back(Token::CONST, buf);
            continue;
        }

        if (ch == '='){
            int n = peek();
            if (n == '=') { 
                getch(); ch = getch(); 
                inputTokens.emplace_back(Token::EQEQ, "=="); 
            }else{            
                ch = getch(); 
                inputTokens.emplace_back(Token::EQ, "=");  
            }
            continue;
        }

        if (ch == '!'){
            int n = peek();
            if (n == '=') { 
                getch(); 
                ch = getch(); 
                inputTokens.emplace_back(Token::NEQ, "!="); 
            }else {
                throw LexicalError{" unknown character"};
            }
            continue;
        }

        if (ch == '<'){
            int n = peek();
            if (n == '=') { 
                getch(); 
                ch = getch(); 
                inputTokens.emplace_back(Token::LE, "<="); 
            }else{
                ch = getch(); 
                inputTokens.emplace_back(Token::LT, "<"); 
            }
            continue;
        }

        if (ch == '>'){
            int n = peek();
            if (n == '='){ 
                getch(); 
                ch = getch(); 
                inputTokens.emplace_back(Token::GE, ">="); 
            }else{
                ch = getch(); 
                inputTokens.emplace_back(Token::GT, ">"); 
            }
            continue;
        }

        if (ch == '+'){ 
            ch = getch(); 
            inputTokens.emplace_back(Token::ADD, "+"); 
            continue; 
        }

        if (ch == '-'){ 
            ch = getch(); 
            inputTokens.emplace_back(Token::SUB, "-"); 
            continue; 
        }

        if (ch == '*'){ 
            ch = getch(); 
            inputTokens.emplace_back(Token::MUL, "*"); 
            continue; 
        }

        if (ch == '/'){
            int n = peek();
            if (n == '/'){ 
                getch(); ch = getch(); 
                inputTokens.emplace_back(Token::FLOORDIV, "//"); 
            }else{            
                ch = getch(); 
                inputTokens.emplace_back(Token::DIV, "/");      
            }
            continue;
        }
        
        if (ch == '.'){
            inputTokens.emplace_back(Token::DOT, ".");
            ch = getch();
            continue;
        }

        if (ch == '%'){
            inputTokens.emplace_back(Token::MOD, "%");
            ch = getch();
            continue;
        }

        if (ch == ','){
            inputTokens.emplace_back(Token::COMMA, ",");
            ch = getch();
            continue;
        }

        if (ch == ';') { 
            ch = getch(); 
            continue; 
        }
        throw LexicalError("Unknown character", row);
    }

    if (!inputTokens.empty() && inputTokens.back().tag != Token::NEWLINE && paren == 0) {
        inputTokens.emplace_back(Token::NEWLINE, "\\n");
    }
    while (S.size() > 1) {
        S.pop_back();
        inputTokens.emplace_back(Token::DEDENT, "DEDENT");
    }
    inputTokens.emplace_back(Token::ENDMARKER, "EOF");
}
