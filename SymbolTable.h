//
// Created by stein on 7/12/2022.
//

#ifndef TRANSPILER_SYMBOLTABLE_H
#define TRANSPILER_SYMBOLTABLE_H


#include <string>
#include <sstream>
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
    bool IsVarInCurrentScope(std::string varName);
    bool IsVarDeclared(std::string varName, std::string varType);
    bool IsVarDeclared(std::string varName);
    bool IsAllowedType(const std::string& compaireType1, const std::string& compaireType2, std::stringstream& errorStream) const;
    void newVar(std::string varName, std::string varType);
    std::string getVarType(std::string varName);
};
#endif //TRANSPILER_SYMBOLTABLE_H
