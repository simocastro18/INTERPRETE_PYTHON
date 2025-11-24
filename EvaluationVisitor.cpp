#include <iostream>
#include "Exception.h"
#include "EvaluationVisitor.h"
#include "Token.h"

//logica di esecuzione che visita i nodi e calcola i valori

static std::string valueToString(const Value& v) {
    switch (v.kind) {
        case Value::INT:  return std::to_string(v.i);
        case Value::BOOL: return v.b ? "True" : "False";
        case Value::LIST: return "<list>";
    }
    return "<invalid>";
}



void EvaluationVisitor::visit(const Program& p) {
    for (Statement* st : p.statements) st->accept(*this);
}

void EvaluationVisitor::visit(const Definition& d) {
    Value v = eval(d.expression_);
    table_.set(d.variable_->id_, v);
    last_ = v; 
}

void EvaluationVisitor::visit(const Print& pr) {
    Value v = eval(pr.arg_);
    out_ << valueToString(v) << std::endl;
    last_ = v;
}

void EvaluationVisitor::visit(const While& w) {
    while(true){
        Value c = eval(w.cond_);
        
        if (c.kind != Value::BOOL)
            throw RuntimeError{" while condition must be bool"};
        if (!c.b) break;

        try {
            for (Statement* st : w.body_) st->accept(*this);
        } catch (const BreakSig&) {
            break;
        }
    }
    last_ = Value::makeInt(0);
}

void EvaluationVisitor::visit(const If& iff) {
    Value c = eval(iff.cond_);
    if (c.kind != Value::BOOL)
        throw RuntimeError{" if condition must be bool"};

    const std::vector<Statement*>& block = c.b ? iff.then_ : iff.els_;
    for (Statement* st : block) st->accept(*this);
    last_ = Value::makeInt(0);
}

void EvaluationVisitor::visit(const ExprStmt& es) {
    last_ = eval(es.expr_);
}

void EvaluationVisitor::visit(const IndexAssign& ia) {
    
    const IndexExpr* lhs = dynamic_cast<const IndexExpr*>(ia.lhs_);
    if (!lhs) throw RuntimeError{" assignment target must be an indexed variable"};


    const Variable* recvVar = dynamic_cast<const Variable*>(lhs->receiver_);
    if (!recvVar) throw RuntimeError{" assignment target must be a variable (list)"};


    Value cont = table_.get(recvVar->id_);
    if (cont.kind != Value::LIST)
        throw RuntimeError{" indexing requires a list"};

    Value idx = eval(lhs->index_);
    if (idx.kind != Value::INT) throw RuntimeError{" index must be int"};
    long long i = idx.i;
    if (i < 0 || i >= static_cast<long long>(cont.v.size()))
        throw RuntimeError{" index out of range"};

    Value val = eval(ia.value_);
    if (val.kind != Value::INT)
        throw RuntimeError{" list elements must be int"};

    cont.v[static_cast<size_t>(i)] = val.i;
    table_.set(recvVar->id_, cont);
    last_ = Value::makeInt(0);
}

void EvaluationVisitor::visit(const Break&) {
    throw BreakSig{};
}


void EvaluationVisitor::visit(const Variable& v) {
    last_ = table_.get(v.id_);
}

void EvaluationVisitor::visit(const Constant& c) {
    last_ = Value::makeInt(c.num_);
}

void EvaluationVisitor::visit(const BoolConst& b) {
    last_ = Value::makeBool(b.val_);
}

void EvaluationVisitor::visit(const Operator& op) {
    if (op.opCode_ == Token::NOT) {
        Value r = eval(op.left_);
        last_ = Value::makeBool(!as_bool(r));
        return;
    }

    Value a = eval(op.left_);
    Value b = eval(op.right_);

    switch (op.opCode_) {
        
        case Token::ADD: {
            long long x = as_int(a), y = as_int(b);
            last_ = Value::makeInt(x + y);
            break;
        }
        case Token::SUB: {
            long long x = as_int(a), y = as_int(b);
            last_ = Value::makeInt(x - y);
            break;
        }
        case Token::MUL: {
            long long x = as_int(a), y = as_int(b);
            last_ = Value::makeInt(x * y);
            break;
        }
        case Token::DIV: {
            long long x = as_int(a), y = as_int(b);
            if (y == 0) throw RuntimeError{" division by zero"};
            last_ = Value::makeInt(x / y);
            break;
        }
        case Token::FLOORDIV: {
            long long x = as_int(a), y = as_int(b);
            if (y == 0) throw RuntimeError{" floor division by zero"};
            last_ = Value::makeInt(x / y);
            break;
        }
        case Token::MOD: {
            long long x = as_int(a), y = as_int(b);
            if (y == 0) throw RuntimeError{" modulo by zero"};
            last_ = Value::makeInt(x % y);
            break;
        }
        case Token::EQEQ: {
            if (a.kind != b.kind) { last_ = Value::makeBool(false); break; }
            if (a.kind == Value::INT)  last_ = Value::makeBool(a.i == b.i);
            else if (a.kind == Value::BOOL) last_ = Value::makeBool(a.b == b.b);
            else last_ = Value::makeBool(false);
            break;
        }
        case Token::NEQ: {
            if (a.kind != b.kind) { last_ = Value::makeBool(true); break; }
            if (a.kind == Value::INT)  last_ = Value::makeBool(a.i != b.i);
            else if (a.kind == Value::BOOL) last_ = Value::makeBool(a.b != b.b);
            else last_ = Value::makeBool(true);
            break;
        }
        case Token::LT:  last_ = Value::makeBool(as_int(a) <  as_int(b)); break;
        case Token::LE:  last_ = Value::makeBool(as_int(a) <= as_int(b)); break;
        case Token::GT:  last_ = Value::makeBool(as_int(a) >  as_int(b)); break;
        case Token::GE:  last_ = Value::makeBool(as_int(a) >= as_int(b)); break;

        
        case Token::AND: {
            bool x = as_bool(a);
            if (!x) { last_ = Value::makeBool(false); break; }
            bool y = as_bool(b);
            last_ = Value::makeBool(x && y);
            break;
        }
        case Token::OR: {
            bool x = as_bool(a);
            if (x) { last_ = Value::makeBool(true); break; }
            bool y = as_bool(b);
            last_ = Value::makeBool(x || y);
            break;
        }

        default:
            throw RuntimeError{" operator not supported in evaluator"};
    }
}

void EvaluationVisitor::visit(const IndexExpr& ix) {
    Value recv = eval(ix.receiver_);
    if (recv.kind != Value::LIST)
        throw RuntimeError{" indexing requires a list"};

    Value idv = eval(ix.index_);
    if (idv.kind != Value::INT) throw RuntimeError{" index must be int"};

    long long i = idv.i;
    if (i < 0 || i >= static_cast<long long>(recv.v.size()))
        throw RuntimeError{" index out of range"};

    last_ = Value::makeInt(recv.v[static_cast<size_t>(i)]);
}

void EvaluationVisitor::visit(const CallExpr& c) {
    if (c.name_ == "list") {
        if (c.args_.size() == 0) {
            Value L = Value::makeList();
            last_ = L;
            return;
        }
        if (c.args_.size() == 1) {
            long long n = as_int(eval(c.args_[0]));
            if (n < 0) throw RuntimeError{" negative size"};
            Value L = Value::makeList();
            L.v.assign(static_cast<size_t>(n), 0);
            last_ = L;
            return;
        }
        throw RuntimeError{" list(n) expects 0 or 1 argument"};
    }

    if (c.name_ == "len"){
        if (c.args_.size() != 1){
            throw RuntimeError{" len(x): expects 1 argument"};
        }
        Value x = eval(c.args_[0]);
        if (x.kind != Value::LIST){
            throw RuntimeError{" len() expects a list"};
        }
        last_ = Value::makeInt(static_cast<long long>(x.v.size()));
        return;
    }
    throw RuntimeError{" unknown function: " + c.name_};
}

void EvaluationVisitor::visit(const MethodCallExpr& mc) {
    Value recv = eval(mc.recv_);
    if (recv.kind != Value::LIST){
        throw RuntimeError{" method call requires a list"};
    }
    if (mc.method_ == "append"){
        if (mc.args_.size() != 1){
            throw RuntimeError{" append expects 1 argument"};
        } 
        Value val = eval(mc.args_[0]);
        if (val.kind != Value::INT){
            throw RuntimeError{" list elements must be int"};
        }
        recv.v.push_back(val.i);
        if (const Variable* v = dynamic_cast<const Variable*>(mc.recv_)) {
            table_.set(v->id_, recv);
        }
        last_ = Value::makeInt(0);
        return;
    }
    throw RuntimeError{" unknown list method: " + mc.method_};
}
