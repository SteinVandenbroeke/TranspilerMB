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

bool SymbolTable::IsVarInCurrentScope(std::string varName, std::string varType) {
    if(table.find(varName) != table.end() && table[varName] == varType){
        return true;
    }
    return false;
}

bool SymbolTable::IsVarDeclared(std::string varName, std::string varType) {
    if(table.find(varName) != table.end() && table[varName] == varType){
        return true;
    }
    else if(outerScope == nullptr){
        return false;
    }
    return outerScope->IsVarDeclared(varName, varType);
}


void SymbolTable::newVar(std::string varName, std::string varType) {
    this->table[varName] = varType;
}

bool SymbolTable::checkVarType() {

    return false;
}
