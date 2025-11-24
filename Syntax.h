#if !defined(SYNTAX_H)
#define SYNTAX_H

#include <vector>
#include <string>
#include <utility>

//definizioni dei nodi dell ast

class Visitor;



struct Statement {
    virtual ~Statement() = default;
    virtual void accept(Visitor& visitor) const = 0;
};

struct Expression : public Statement { };




struct Program {
    Program() = default;
    Program(std::vector<Statement*> stmts) : statements{std::move(stmts)} {}
    ~Program();
    void accept(Visitor& visitor) const;
    
    std::vector<Statement*> statements;
};




struct Operator : public Expression {
    
    Operator(int opCode, Expression* l, Expression* r)
        : opCode_{ opCode }, left_{ l }, right_{ r } { }
    ~Operator();

    void accept(Visitor& visitor) const override;

    int opCode_;
    Expression* left_;
    Expression* right_;
};

struct Variable : public Expression {
    explicit Variable(std::string id) : id_{ std::move(id) } { }

    void accept(Visitor& visitor) const override;

    std::string id_;
};

struct Constant : public Expression {
    explicit Constant(int num) : num_{ num } { }

    void accept(Visitor& visitor) const override;

    int num_;
};


struct CallExpr : public Expression {
    CallExpr(std::string name, std::vector<Expression*> args)
        : name_{ std::move(name) }, args_{ std::move(args) } {}

    ~CallExpr();

    void accept(Visitor& visitor) const override;

    std::string name_;
    std::vector<Expression*> args_;
};




struct Definition : public Statement {
    Definition(Variable* v, Expression* e) : variable_{ v }, expression_{ e } { }
    ~Definition();

    void accept(Visitor& visitor) const override;

    Variable*   variable_; 
    Expression* expression_; 
};


struct Print : public Statement {
    explicit Print(Expression* arg) : arg_{ arg } {}
    ~Print();

    void accept(Visitor& visitor) const override;

    Expression* arg_; 
};


struct While : public Statement {
    While(Expression* cond, std::vector<Statement*> body)
        : cond_{ cond }, body_{ std::move(body) } {}
    ~While();

    void accept(Visitor& visitor) const override;

    Expression* cond_;                 
    std::vector<Statement*> body_;    
};

struct ExprStmt : public Statement {
    explicit ExprStmt(Expression* e) : expr_{ e } {}
    ~ExprStmt();

    void accept(Visitor& visitor) const override;

    Expression* expr_; 
};

struct MethodCallExpr : public Expression {
    MethodCallExpr(Variable* recv, std::string method, std::vector<Expression*> args)
        : recv_{ recv }, method_{ std::move(method) }, args_{ std::move(args) } {}
    ~MethodCallExpr();  
    Variable* recv_;
    std::string method_;
    std::vector<Expression*> args_;
    void accept(Visitor& v) const override;
};


struct IndexExpr : public Expression {
    IndexExpr(Variable* recv, Expression* idx)
        : receiver_{recv}, index_{idx} {}
    ~IndexExpr();
    void accept(Visitor& v) const override;
    Variable*   receiver_; 
    Expression* index_;    
};



struct IndexAssign : public Statement {
    IndexAssign(IndexExpr* recv_idx, Expression* value)
        : lhs_{recv_idx}, value_{value} {}
    ~IndexAssign();
    void accept(Visitor& v) const override;
    IndexExpr*  lhs_;    
    Expression* value_;  
};


struct If : public Statement {
    If(Expression* cond, std::vector<Statement*> thenb, std::vector<Statement*> elseb)
        : cond_{cond}, then_{std::move(thenb)}, els_{std::move(elseb)} {}
    ~If();
    void accept(Visitor& v) const override;
    Expression* cond_;               
    std::vector<Statement*> then_;   
    std::vector<Statement*> els_;    
};


struct Break : public Statement {
    void accept(Visitor& v) const override;
    ~Break() = default;
};

struct BoolConst : public Expression {
    explicit BoolConst(bool val) : val_{val} {}
    void accept(Visitor& visitor) const override;
    bool val_;
};


#endif
