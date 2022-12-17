//
// Created by stein on 29/11/2022.
//

#ifndef TRANSPILER_ASTNODE_H
#define TRANSPILER_ASTNODE_H
#include <vector>
#include "tokenizer/Token.h"
#include <string>
#include "SymbolTable.h"
#include <iostream>
#include <sstream>

///forward declarations
class AstProgramLine;
class AstDeclartion;
class AstIntalisation;
class AstWhile;
class AstIf;
class AstBody;
class AstCondition;
class AstBoolOperator;
class AstArithmeticOperations;
class AstVar;
class AstNumber;
class AstString;
class AstChar;
class AstProgram;
class AstValue;
class AstVarOrValue;
class AstParentheses;
class AstPrint;
///forward declarations

static int identifierCounter = 0;

class AstNode {
protected:
    Token* token;
    int identifier;
    std::string generateDotInternal() const;
public:
    AstNode(Token* token);
    AstNode();
    std::string getTokenType() const;
    std::string getTokenText() const;
    virtual std::string getValue() const;
    std::string getIdentifier() const;
    std::string generateDOT() const;
    virtual std::string getJsCode(int scopeCount) const = 0;
    virtual std::string getType(SymbolTable& table) const;
    virtual bool checkTypes(SymbolTable& table) const;
    virtual std::vector<AstNode*> getChilderen() const;
};

class AstProgram: public AstNode{
protected:
    std::vector<AstNode*> lines;
public:
    void addLine(AstNode* programLine);
    std::string getJsCode(int scopeCount = 0) const override;
    std::vector<AstNode*> getChilderen() const override;
    bool checkTypes();
    std::string getValue() const;
};

/**
 * AstDeclartion node to represent declaration
 * structure:
 *     [this]
 *    /     \
 * AstVar  AstValue
 *
 *
 * vb: int i = 10;
 *      int
 *    /     \
 * AstVar  AstValue
 */
class AstDeclartion:public AstNode{
    AstVar* var = nullptr;
    AstNode* value = nullptr;
public:
    AstDeclartion(Token* token);
    AstDeclartion(Token* token, AstVar* var, AstNode* value);
    void setAstVar(AstVar* var);
    void setAstValue(AstNode* value);
    AstVar * getAstVar();
    AstNode* getAstValue();
    std::vector<AstNode*> getChilderen() const override;
    std::string getJsCode(int scopeCount) const override;
    bool checkTypes(SymbolTable& table) const override;
};

/**
 * AstIntalisations node to represent variable initialisation
 * structure:
 *     [this]
 *    /     \
 * AstVar  AstValue
 *
 *
 * vb: i = 10;
 *       =
 *    /     \
 * AstVar  AstValue
 */
class AstIntalisation: public AstNode{
    AstVar* var = nullptr;
    AstNode* value = nullptr;
public:
    AstIntalisation(Token* token);
    AstIntalisation(Token* token, AstVar* var, AstNode* value);
    void setAstVar(AstVar* var);
    void setAstValue(AstValue* value);
    AstVar* getAstVar();
    AstNode* getAstValue();
    std::vector<AstNode*> getChilderen() const override;
    std::string getJsCode(int scopeCount) const override;
    bool checkTypes(SymbolTable& table) const override;
};

class AstPrint: public AstNode{
    AstNode* value = nullptr;
public:
    AstPrint(Token* token);
    AstPrint(Token* token, AstNode *value);
    std::vector<AstNode*> getChilderen() const override;
    std::string getJsCode(int scopeCount) const override;
};

class AstConditionBody: public AstNode{
protected:
    AstCondition* condition = nullptr;
    AstBody* body = nullptr;
public:
    AstConditionBody(Token* token);
    AstConditionBody(Token* token,AstCondition* condition, AstBody* body);
    void setAstCondition(AstCondition* condition);
    void setAstBody(AstBody* body);
    AstCondition* getAstCondition();
    AstBody* getAstBody();
    std::vector<AstNode*> getChilderen() const override;
    virtual std::string getJsCode(int scopeCount) const override = 0;
};

/**
 * AstWhile node to represent a while loop
 * structure:
 *           [this]
 *          /     \
 * AstCondition  AstBody
 *
 *
 * vb: while(i < 10){ body }
 *           while
 *          /     \
 * AstCondition  AstBody
 */
class AstWhile: public AstConditionBody{
public:
    AstWhile(Token* token,AstCondition *condition, AstBody *body);
    std::string getJsCode(int scopeCount) const override;
    std::string getValue() const override;
};

/**
 * AstIf node to represent a if statement
 * structure:
 *           [this]
 *          /     \
 * AstCondition  AstBody
 *
 *
 * vb: if(i < 10){ body }
 *             if
 *          /     \
 * AstCondition  AstBody
 */
class AstIf:public AstConditionBody{
public:
    AstIf(Token* token, AstCondition *condition, AstBody *body);
    std::string getJsCode(int scopeCount) const override;
};

/**
 * AstBody node to represent a body
 * structure:
 *           [this]
 */
class AstBody:public AstProgram{
public:
    std::string getJsCode(int scopeCount) const override;
    bool checkTypes(SymbolTable& table) const override;
};

/**
 * AstCondition node to represent a condition
 * structure:
 *           [this]
 *          /     \
 * AstVarOrValue  AstVarOrValue
 *
 *
 * vb: i == 10
 *             ==
 *          /     \
 * AstVarOrValue  AstVarOrValue
 */
class AstCondition:public AstNode{
    AstNode* val1;
    AstNode* val2;
public:
    AstCondition(Token* token);
    AstCondition(Token* token, AstNode* var1, AstNode* var2);
    void setVar1(AstNode* valOrValue);
    void setVar2(AstNode* valOrValue);
    std::vector<AstNode*> getChilderen() const override;
    std::string getJsCode(int scopeCount) const override;
    bool checkTypes(SymbolTable& table) const override;
};

class AstArithmeticOperations:public AstNode{
    AstNode* val1;
    AstNode* val2;
public:
    AstArithmeticOperations(Token* token);
    AstArithmeticOperations(Token* token, AstNode* val1, AstNode* val2);
    void setVal1(AstNode* valOrValue);
    void setVal2(AstNode* valOrValue);
    std::vector<AstNode*> getChilderen() const override;
    std::string getJsCode(int scopeCount) const override;
    std::string getType(SymbolTable& table) const override;
    bool checkTypes(SymbolTable& table) const override;
};

class AstParentheses:public AstNode{
    AstNode* innerNode;
public:
    AstParentheses(Token* token, AstNode* innerNode);
    std::vector<AstNode*> getChilderen() const override;
    std::string getJsCode(int scopeCount) const override;
    std::string getType(SymbolTable& table) const override;
    std::string getValue() const override;
};

class AstVarOrValue:public AstNode{
public:
    AstVarOrValue(Token* token);
    virtual std::string getJsCode(int scopeCount) const override = 0;
    virtual std::string getType(SymbolTable& table) const override = 0;
};

class AstVar:public AstVarOrValue{
public:
    AstVar(Token* token);
    std::string getJsCode(int scopeCount) const override;
    std::string getType(SymbolTable& table) const override;
    bool checkTypes(SymbolTable& table) const override;
};

class AstValue:public AstVarOrValue{
public:
    AstValue(Token* token);
    std::string getJsCode(int scopeCount) const override;
    std::string getType(SymbolTable& table) const override;
};
#endif //TRANSPILER_ASTNODE_H
