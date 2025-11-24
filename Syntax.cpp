#include "Syntax.h"
#include "Visitor.h"

//distruttori e metodi di supporto dei nodi dell ast

Program::~Program(){
    for (auto* s : statements){
        delete s;
    }
}

void Program::accept(Visitor& v) const {v.visit(*this);}


Operator::~Operator(){
    delete left_;
    delete right_;
}
void Operator::accept(Visitor& v) const {v.visit(*this);}


void Variable::accept(Visitor& v) const {v.visit(*this);}


void Constant::accept(Visitor& v) const {v.visit(*this);}


Definition::~Definition() {
    delete variable_;
    delete expression_;
}
void Definition::accept(Visitor& v) const {v.visit(*this);}


Print::~Print() {delete arg_;}
void Print::accept(Visitor& v) const {v.visit(*this);}


While::~While() {
    delete cond_;
    for (auto* s : body_) delete s;
}
void While::accept(Visitor& v) const {v.visit(*this);}


CallExpr::~CallExpr(){
    for (auto* e : args_) delete e;
}
void CallExpr::accept(Visitor& v) const {v.visit(*this);}

ExprStmt::~ExprStmt() {delete expr_;}
void ExprStmt::accept(Visitor& v) const {v.visit(*this);}

MethodCallExpr::~MethodCallExpr() {
    delete recv_;
    for (auto* e : args_) delete e;
}
void MethodCallExpr::accept(Visitor& v) const {v.visit(*this);}


IndexExpr::~IndexExpr() {delete receiver_; delete index_;}
void IndexExpr::accept(Visitor& v) const {v.visit(*this);}


IndexAssign::~IndexAssign() {delete lhs_; delete value_;}
void IndexAssign::accept(Visitor& v) const {v.visit(*this);}


If::~If() { 
    delete cond_; 
    for (auto* s: then_) {
        delete s;
    }
    for (auto* s: els_){
        delete s;
    } 
}
void If::accept(Visitor& v) const {v.visit(*this);}

void Break::accept(Visitor& v) const {v.visit(*this);}

void BoolConst::accept(Visitor& v) const { v.visit(*this); }


