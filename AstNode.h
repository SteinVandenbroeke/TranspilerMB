//
// Created by stein on 29/11/2022.
//

#ifndef TRANSPILER_ASTNODE_H
#define TRANSPILER_ASTNODE_H
#include <vector>
#include "Token.h"
#include <string>
#include "SymbolTable.h"
#include <iostream>

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
///forward declarations

enum astNodeType {AstProgramC, AstDeclartionC, AstIntalisationsC, AstWhileC, AstIfC,AstBodyC,
        AstConditionC,AstBoolOperatorC,AstArithmeticOperationsC,AstVarC,AstNumberC,AstStringC,AstCharC,
        AstValueC, AstVarOrValueC};

class AstNode {
protected:
    Token* token;
public:
    AstNode(Token* token);
    AstNode();
    std::string getTokenType();
    std::string getTokenText();
    virtual std::string getJsCode() = 0;
    virtual astNodeType getType() = 0;
    virtual bool checkTypes(SymbolTable& table);
    virtual std::vector<AstNode*> getChilderen();
};

class AstProgram: public AstNode{
    std::vector<AstNode*> lines;
public:
    void addLine(AstNode* programLine);
    std::string getJsCode() override;
    astNodeType getType() override;
    bool checkTypes();
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
    AstValue* value = nullptr;
public:
    AstDeclartion(Token* token);
    AstDeclartion(Token* token, AstVar* var, AstValue* value);
    void setAstVar(AstVar* var);
    void setAstValue(AstValue* value);
    AstVar * getAstVar();
    AstValue* getAstValue();
    std::vector<AstNode*> getChilderen() override;
    std::string getJsCode() override;
    astNodeType getType() override;
    bool checkTypes(SymbolTable& table) override;
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
    AstIntalisation();
    AstIntalisation(AstVar* var, AstNode* value);
    void setAstVar(AstVar* var);
    void setAstValue(AstValue* value);
    AstVar* getAstVar();
    AstNode* getAstValue();
    std::vector<AstNode*> getChilderen() override;
    std::string getJsCode() override;
    astNodeType getType() override;
};

class AstConditionBody: public AstNode{
protected:
    AstCondition* condition = nullptr;
    AstBody* body = nullptr;
public:
    AstConditionBody();
    AstConditionBody(AstCondition* condition, AstBody* body);
    void setAstCondition(AstCondition* condition);
    void setAstBody(AstBody* body);
    AstCondition* getAstCondition();
    AstBody* getAstBody();
    std::vector<AstNode*> getChilderen() override;
    virtual std::string getJsCode() = 0;
    virtual astNodeType getType() = 0;
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
    AstWhile(AstCondition *condition, AstBody *body);
    std::string getJsCode() override;
    astNodeType getType() override;
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
    AstIf(AstCondition *condition, AstBody *body);

    std::string getJsCode() override;
    astNodeType getType() override;
};

/**
 * AstBody node to represent a body
 * structure:
 *           [this]
 */
class AstBody:public AstProgram{
public:
    std::string getJsCode() override;
    astNodeType getType() override;
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
    std::vector<AstNode*> getChilderen() override;
    std::string getJsCode() override;
    astNodeType getType() override;
};

class AstArithmeticOperations:public AstNode{
    AstNode* val1;
    AstNode* val2;
public:
    AstArithmeticOperations(Token* token);
    AstArithmeticOperations(Token* token, AstNode* val1, AstNode* val2);
    void setVal1(AstNode* valOrValue);
    void setVal2(AstNode* valOrValue);
    std::vector<AstNode*> getChilderen() override;
    std::string getJsCode() override;
    astNodeType getType() override;
};

class AstVarOrValue:public AstNode{
public:
    AstVarOrValue(Token* token);
};

class AstVar:public AstVarOrValue{
public:
    AstVar(Token* token);
    std::string getJsCode() override;
    astNodeType getType() override;
    bool checkTypes(SymbolTable& table) override;
};

class AstValue:public AstVarOrValue{
public:
    AstValue(Token* token);
    std::string getJsCode() override;
    astNodeType getType() override;
};
#endif //TRANSPILER_ASTNODE_H
