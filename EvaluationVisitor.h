#ifndef EVALUATIONVISITOR_H
#define EVALUATIONVISITOR_H


#include <iostream>
#include "Visitor.h"
#include "SymbolTable.h"
#include "Exception.h"
#include "Syntax.h"

//dichiarazioni del visitor che esegue il programma

class EvaluationVisitor : public Visitor {
public:
    EvaluationVisitor(SymbolTable& table, std::ostream& out): table_(table), out_(out) {}

    void visit(const Program&) override;
    void visit(const Definition&) override;
    void visit(const Print&) override;
    void visit(const While&) override;
    void visit(const ExprStmt&) override;
    void visit(const If&) override;
    void visit(const IndexAssign&) override;
    void visit(const BoolConst&) override;

    void visit(const Operator&) override;
    void visit(const Variable&) override;
    void visit(const Constant&) override;
    void visit(const CallExpr&) override;
    void visit(const MethodCallExpr&) override;
    void visit(const IndexExpr&) override;
    void visit(const Break&) override;

    Value last() const { return last_; }

    Value eval(const Expression* e) {
        if (!e) throw RuntimeError{" null expression"};
        e->accept(*this);
        return last_;
    }

private:
    SymbolTable& table_;
    std::ostream& out_;
    Value last_{ Value::makeInt(0) };
    struct BreakSig : std::exception {};
    static long long as_int(const Value& v) {
        if (v.kind != Value::INT) throw RuntimeError{" expected int"};
        return v.i;
    }
    static bool as_bool(const Value& v) {
        if (v.kind != Value::BOOL) throw RuntimeError{" expected bool"};
        return v.b;
    }
    
};
#endif