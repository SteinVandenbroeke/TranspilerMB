//
// Created by stein on 7/12/2022.
//

#ifndef TRANSPILER_SYMBOLTABLE_H
#define TRANSPILER_SYMBOLTABLE_H


#include <string>
#include <map>

class SymbolTable {
    SymbolTable* outerScope = nullptr;
    std::map<std::string, bool> table;
public:
    SymbolTable();
    SymbolTable(SymbolTable* outerScope);
    SymbolTable* newScope();
    SymbolTable* removeScope();
    bool IsVarInCurrentScope(std::string varName);
    bool IsVarDeclared(std::string varName);
    bool IsVarInitialized(std::string varName);
    void newVar(std::string varName);
    void InitializedVar(std::string varName);
};
#endif //TRANSPILER_SYMBOLTABLE_H
