#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <unordered_map>
#include <string>
#include <vector>
#include "Exception.h"

//tabella dei simboli per variabili e liste

struct Value {
    enum Kind { INT, BOOL, LIST } kind = INT;
    long long i = 0;              
    bool b = false;               
    std::vector<long long> v;       

    static Value makeInt(long long x){ Value t; t.kind=INT;  t.i=x; return t; }
    static Value makeBool(bool x){ Value t; t.kind=BOOL; t.b=x; return t; }
    static Value makeList(){ Value t; t.kind=LIST; return t; }
};

class SymbolTable {
public:
    Value get(const std::string& id);
    void set(const std::string& id, const Value& value);
private:
    std::unordered_map<std::string, Value> variable_container;
};

#endif // SYMBOLTABLE_H