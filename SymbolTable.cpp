//
// Created by stein on 7/12/2022.
//

#include "SymbolTable.h"

SymbolTable::SymbolTable() {

}

SymbolTable::SymbolTable(SymbolTable *outerScope) {
    this->outerScope = outerScope;
}

SymbolTable *SymbolTable::newScope() {
    return new SymbolTable(outerScope);
}

SymbolTable *SymbolTable::removeScope() {
    SymbolTable* outerScope = this->outerScope;
    delete this;
    return outerScope;
}

bool SymbolTable::IsVarInCurrentScope(std::string varName) {
    if(table.find(varName) != table.end()){
        return true;
    }
    return false;
}

bool SymbolTable::IsVarDeclared(std::string varName) {
    if(table.find(varName) != table.end()){
        return true;
    }
    return outerScope->IsVarDeclared(varName);
}

bool SymbolTable::IsVarInitialized(std::string varName) {
    if(table.find(varName) != table.end()){
        return table[varName];
    }
    return outerScope->IsVarInitialized(varName);
}

void SymbolTable::newVar(std::string varName) {
    this->table[varName] = false;
}

void SymbolTable::InitializedVar(std::string varName) {
    this->table[varName] = true;
}
