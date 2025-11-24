#include "Parser.h"
#include <cstdlib> 

//analizza la sequenza di token e costruisce l ast

bool Parser::read_token(int tag){
    if (!last_token() && current_token().tag == tag) { 
        ++pos; 
        return true; 
    }
    return false;
}

const Token& Parser::expect(int tag, const char* msg){
    if (!last_token() && current_token().tag == tag){ 
        return (*tokens)[pos++]; 
    }
    throw SyntaxError{ msg };
}

void Parser::expect_newline(){
    if (!read_token(Token::NEWLINE)){
        throw SyntaxError{" expected NEWLINE"};
    }
    while (read_token(Token::NEWLINE)){}
}

std::vector<Statement*> Parser::parse_block() {
    expect_newline();
    expect(Token::INDENT, "expected INDENT after ':'");
    auto body = parse_stmt_list();
    expect(Token::DEDENT, "expected DEDENT to close block");
    return body;
}

Variable* Parser::parse_variable() {
    if (last_token() || current_token().tag != Token::ID) {
        throw SyntaxError{" expected identifier"};
    }
    const Token& id = (*tokens)[pos++];
    return new Variable(id.word);
}

// START
Program* Parser::parse_program(){
    Program* prog = new Program{};
    while (read_token(Token::NEWLINE)){}
    (*prog).statements = parse_stmt_list();
    expect(Token::ENDMARKER, "expected end of file");
    return prog;
}

std::vector<Statement*> Parser::parse_stmt_list(){
    std::vector<Statement*> store_stmt;
    while (!last_token() && current_token().tag != Token::DEDENT && current_token().tag != Token::ENDMARKER){
        if (current_token().tag == Token::NEWLINE){ 
            ++pos; 
            continue; 
        }
        store_stmt.push_back(parse_stmt());
    }
    return store_stmt;
}

Statement* Parser::parse_stmt(){
    if (current_token().tag == Token::WHILE){
        return parse_while();
    }
    if (current_token().tag == Token::IF){
        return parse_if();
    }

    auto s = parse_simple_stmt();
    expect_newline();
    return s;
}

Statement* Parser::parse_simple_stmt(){
    
    if (current_token().tag == Token::PRINT){
        return parse_print();
    }
    
    if (current_token().tag == Token::BREAK){
        return parse_break();
    }
    
    if (current_token().tag == Token::ID) {
        if (pos + 1 < tokens->size()) {
            int t2 = (*tokens)[pos + 1].tag;

            if (t2 == Token::EQ){
                return parse_assignment();
            }
            if (t2 == Token::LBRACK){
                return parse_index_assign();
            }
            if (t2 == Token::DOT){   
                const Token& idTok = expect(Token::ID, "expected identifier");
                expect(Token::DOT, "expected '.' after identifier");
                
                const Token& meth = expect(Token::ID, "expected method name after '.'");
                if (meth.word != "append")
                    throw SyntaxError{" only append(...) is allowed here"};

                expect(Token::LP, "expected '(' after append");
                std::vector<Expression*> args;
                if (!read_token(Token::RP)) {
                    args.push_back(parse_expr());
                    while (read_token(Token::COMMA)) args.push_back(parse_expr());
                    expect(Token::RP, "expected ')'");
                }
                if (args.size() != 1){
                    throw SyntaxError{" append() expects exactly one argument"};
                }
                
                Variable* recv = new Variable(idTok.word);
                Expression* call = new MethodCallExpr(recv, "append", std::move(args));
                return new ExprStmt(call);
            }
        }
        throw SyntaxError{" after identifier expected '=', '[' or '.'"};
    }
    throw SyntaxError{" expected simple statement"};
}

Statement* Parser::parse_assignment(){
    const Token& id = expect(Token::ID, "expected identifier");
    expect(Token::EQ, "expected '=' after identifier");
    Expression* e = parse_expr();
    return new Definition(new Variable(id.word), e);
}

Statement* Parser::parse_print(){
    expect(Token::PRINT, "expected 'print'");
    expect(Token::LP, "expected '(' after print");
    Expression* e = parse_expr();
    expect(Token::RP, "expected ')'");
    return new Print(e);
}

Statement* Parser::parse_while(){
    expect(Token::WHILE, "expected 'while'");
    expect(Token::LP, "expected '(' after while");
    Expression* cond = parse_expr();
    expect(Token::RP, "expected ')'");
    expect(Token::COLON, "expected ':' after while(...)");
    auto body = parse_block(); 
    return new While(cond, std::move(body));
}

Statement* Parser::parse_if(){
    expect(Token::IF, "expected 'if'");
    expect(Token::LP, "expected '(' after if");
    Expression* cond = parse_expr();
    expect(Token::RP, "expected ')'");
    expect(Token::COLON, "expected ':' after if(...)");
    auto thenb = parse_block();

    If* root = new If(cond, std::move(thenb), {});
    If* tail = root;
  
    while (!last_token() && current_token().tag == Token::ELIF) {
        ++pos; 
        expect(Token::LP, "expected '(' after elif");
        Expression* c2 = parse_expr();
        expect(Token::RP, "expected ')'");
        expect(Token::COLON, "expected ':' after elif(...)");
        auto b2 = parse_block();
        If* node = new If(c2, std::move(b2), {});
        tail->els_.push_back(node);  
        tail = node;                 
    }

    if (!last_token() && current_token().tag == Token::ELSE) {
        ++pos; // consuma 'else'
        expect(Token::COLON, "expected ':' after else");
        tail->els_ = parse_block();
    }

    return root;
}

Statement* Parser::parse_index_assign(){
    Variable* recv = parse_variable();                 
    expect(Token::LBRACK, "expected '['");
    Expression* idx = parse_expr();
    expect(Token::RBRACK, "expected ']'");

    IndexExpr* lhs = new IndexExpr(recv, idx);        
    expect(Token::EQ, "expected '=' after index lhs");

    Expression* value = parse_expr();
    return new IndexAssign(lhs, value);              
}

Statement* Parser::parse_break(){
    expect(Token::BREAK, "expected 'break'");
    return new Break{};
}


Expression* Parser::parse_expr(){ 
    return parse_or(); 
}

Expression* Parser::parse_or(){
    Expression* left = parse_and();
    while (read_token(Token::OR)) {
        Expression* right = parse_and();
        left = new Operator(Token::OR, left, right);
    }
    return left;
}

Expression* Parser::parse_and(){
    Expression* left = parse_not();
    while (read_token(Token::AND)){
        Expression* right = parse_not();
        left = new Operator(Token::AND, left, right);
    }
    return left;
}

Expression* Parser::parse_not(){
    if (read_token(Token::NOT)){
        Expression* arg = parse_not();
        return new Operator(Token::NOT, arg, nullptr);
    }
    return parse_rel();
}

Expression* Parser::parse_rel(){
    Expression* left = parse_add();
    while (true) {
        if (read_token(Token::EQEQ)){ 
            Expression* r = parse_add(); 
            left = new Operator(Token::EQEQ, left, r); 
        }else if (read_token(Token::NEQ)){ 
            Expression* r = parse_add(); 
            left = new Operator(Token::NEQ, left, r); 
        }else if (read_token(Token::LT)){ 
            Expression* r = parse_add(); 
            left = new Operator(Token::LT,  left, r); 
        }else if (read_token(Token::LE)){ 
            Expression* r = parse_add(); 
            left = new Operator(Token::LE,  left, r); 
        }else if (read_token(Token::GT)){ 
            Expression* r = parse_add(); 
            left = new Operator(Token::GT,  left, r); 
        }else if (read_token(Token::GE)){ 
            Expression* r = parse_add(); 
            left = new Operator(Token::GE,  left, r); 
        }else break;
    }
    return left;
}

Expression* Parser::parse_add() {
    Expression* left = parse_mul();
    while (true) {
        if (read_token(Token::ADD)){ 
            Expression* r = parse_mul(); 
            left = new Operator(Token::ADD, left, r); 
        }else if (read_token(Token::SUB)){ 
            Expression* r = parse_mul(); 
            left = new Operator(Token::SUB, left, r); 
        }else break;
    }
    return left;
}

Expression* Parser::parse_mul() {
    Expression* left = parse_unary();
    while (true) {
        if (read_token(Token::MUL)){ 
            Expression* r = parse_unary(); 
            left = new Operator(Token::MUL, left, r); 
        }else if (read_token(Token::DIV)){ 
            Expression* r = parse_unary(); 
            left = new Operator(Token::DIV, left, r); 
        }else if (read_token(Token::FLOORDIV)){ 
            Expression* r = parse_unary(); 
            left = new Operator(Token::FLOORDIV, left, r); 
        }else if (read_token(Token::MOD)){ 
            Expression* r = parse_unary(); 
            left = new Operator(Token::MOD, left, r); 
        }else break;
    }
    return left;
}

Expression* Parser::parse_unary() {
    if (read_token(Token::SUB)){
        Expression* arg = parse_unary();
        return new Operator(Token::SUB, new Constant(0), arg);
    }
    return parse_atom();
}

Expression* Parser::parse_primary() {
    if (read_token(Token::LP)){
        Expression* e = parse_expr();
        expect(Token::RP, "expected ')'");
        return e;
    }
    if (read_token(Token::TRUE)){
        return new BoolConst(true);
    }
    if (read_token(Token::FALSE)){
        return new BoolConst(false);
    }
    if (!last_token() && current_token().tag == Token::CONST) {
        long long v = std::strtoll(current_token().word.c_str(), nullptr, 10); //STRTOLL
        ++pos;
        return new Constant(static_cast<int>(v));
    }
    if (!last_token() && current_token().tag == Token::ID) {
        const Token& id = (*tokens)[pos++];
        if (read_token(Token::LP)) {
            std::vector<Expression*> args;
            if (!read_token(Token::RP)) {
                args.push_back(parse_expr());
                while (read_token(Token::COMMA)) args.push_back(parse_expr());
                expect(Token::RP, "expected ')'");
            }
            return new CallExpr(id.word, std::move(args));
        }
        return new Variable(id.word);
    }

    throw SyntaxError{" expected ID/CONST/(expr)/True/False"};
}

Expression* Parser::parse_atom() {
    if (!last_token() && current_token().tag == Token::ID) {
        const Token& id = (*tokens)[pos++];
        if (read_token(Token::LP)) {
            std::vector<Expression*> args;
            if (!read_token(Token::RP)) {
                args.push_back(parse_expr());
                while (read_token(Token::COMMA)) args.push_back(parse_expr());
                expect(Token::RP, "expected ')'");
            }
            return new CallExpr(id.word, std::move(args));
        }

        Variable* var = new Variable(id.word);
        Expression* e = var;

        while(true){
            if (read_token(Token::LBRACK)) {
                Expression* idx = parse_expr();
                expect(Token::RBRACK, "expected ']'");
                e = new IndexExpr(var, idx);                 
                continue;
            }
            if (read_token(Token::DOT)) {
                const Token& m = expect(Token::ID, "expected method name after '.'");
                expect(Token::LP, "expected '(' after method");
                std::vector<Expression*> args;
                if (!read_token(Token::RP)) {
                    args.push_back(parse_expr());
                    while (read_token(Token::COMMA)) args.push_back(parse_expr());
                    expect(Token::RP, "expected ')'");
                }
                e = new MethodCallExpr(var, m.word, std::move(args)); 
                continue;
            }
            break;
        }
        return e;
    }
    return parse_primary();
}
