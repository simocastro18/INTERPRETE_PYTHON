#include <vector>
#include <memory>
#include <stdexcept>
#include "Token.h"
#include "Syntax.h"
#include "Exception.h"

//dichiarazioni del parser e delle funzioni di parsing

class Parser {
public:
    Program* operator()(std::vector<Token>& toks) {
        tokens = &toks; pos = 0;
        return parse_program();
    }

private:
    std::vector<Token>* tokens = nullptr;
    size_t pos = 0;
    Token& current_token(){ return (*tokens)[pos]; }
    bool last_token() const { return pos >= (*tokens).size(); }
    bool read_token(int tag);
    const Token& expect(int tag, const char* msg);

    Variable* parse_variable();

    void expect_newline();
    std::vector<Statement*> parse_block();

    // produzioni
    Program* parse_program();
    std::vector<Statement*> parse_stmt_list();
    Statement* parse_stmt();               
    Statement* parse_simple_stmt();        
    Statement* parse_assignment();        
    Statement* parse_print();              
    Statement* parse_while();             
    Statement* parse_expr_stmt();         
    Statement* parse_if();                
    Statement* parse_index_assign();     
    Statement* parse_break();
   
    // espressioni
    Expression* parse_expr();  
    Expression* parse_or();
    Expression* parse_and();
    Expression* parse_not();
    Expression* parse_rel();
    Expression* parse_add();
    Expression* parse_mul();
    Expression* parse_unary();
    Expression* parse_atom();
    Expression* parse_primary();
    
};
