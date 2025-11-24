#include "SymbolTable.h"

//implementazione tabella dei simboli

Value SymbolTable::get(const std::string& id){
    auto it = variable_container.find(id);
    if (it == variable_container.end()){
        throw RuntimeError{" undefined variable "};
    }
    return it->second;
}

void SymbolTable::set(const std::string& id, const Value& value){
    variable_container[id] = value;
}

