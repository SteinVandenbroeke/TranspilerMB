//
// Created by stein on 7/12/2022.
//

#ifndef TRANSPILER_SYMBOLTABLE_H
#define TRANSPILER_SYMBOLTABLE_H


#include <string>
#include <map>

class SymbolTable {
    SymbolTable* outerScope = nullptr;
    std::map<std::string, std::string> table = std::map<std::string, std::string>();//<var name, var type>
public:
    SymbolTable();
    SymbolTable(SymbolTable* outerScope);
    SymbolTable* newScope();
    SymbolTable* removeScope();
    bool IsVarInCurrentScope(std::string varName, std::string varType);
    bool IsVarDeclared(std::string varName, std::string varType);
    bool checkVarType();
    void newVar(std::string varName, std::string varType);
};
#endif //TRANSPILER_SYMBOLTABLE_H
