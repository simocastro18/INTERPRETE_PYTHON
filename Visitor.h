//interfaccia visitor usata da ast, parser ed esecutore
struct Program;
struct Definition;
struct Operator;
struct Variable;
struct Constant;
struct Print;
struct While;
struct CallExpr;
struct ExprStmt;
struct MethodCallExpr;
struct If;
struct IndexExpr;
struct IndexAssign;
struct Break; 
struct BoolConst;

class Visitor {
public:
    virtual ~Visitor() = default;

    // Program
    virtual void visit(const Program&)    = 0;
    //Statements
    virtual void visit(const Definition&) = 0;
    virtual void visit(const Print&)      = 0;
    virtual void visit(const While&)      = 0;
    virtual void visit(const ExprStmt&)   = 0;
    virtual void visit(const If&)         = 0; 
    virtual void visit(const IndexAssign&)= 0; 
    virtual void visit(const BoolConst&) = 0;

    // Expressions
    virtual void visit(const Operator&)   = 0;
    virtual void visit(const Variable&)   = 0;
    virtual void visit(const Constant&)   = 0;
    virtual void visit(const CallExpr&)   = 0;
    virtual void visit(const MethodCallExpr&) = 0;
    virtual void visit(const IndexExpr&)  = 0;
    virtual void visit(const Break&) = 0;
};
