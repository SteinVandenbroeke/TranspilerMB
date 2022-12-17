//
// Created by stein on 7/12/2022.
//

#include <iostream>
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

bool SymbolTable::IsVarDeclared(std::string varName) {
    return table.find(varName) != table.end();
}

std::string SymbolTable::getVarType(std::string varName) {
    if(table.find(varName) != table.end()){
        return table[varName];
    }
    else if(outerScope == nullptr){
        return "none";
    }
    return outerScope->getVarType(varName);
}

bool SymbolTable::IsAllowedType(std::string varName, std::string compaireType) {
    if((this->getVarType(varName) == compaireType) || (this->getVarType(varName) == "double" && compaireType == "int") || (this->getVarType(varName) == "string" && compaireType == "char")){
        return true;
    }
    else if(this->getVarType(varName) == "int" && compaireType == "double"){
        std::cout << "implicit conversion double to int" << std::endl;
        return true;
    }
    return false;
}
