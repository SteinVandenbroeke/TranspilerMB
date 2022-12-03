//
// Created by stein on 29/11/2022.
//

#ifndef TRANSPILER_ASTNODE_H
#define TRANSPILER_ASTNODE_H
#include "vector"
#include "Token.h"
#include "string"

///forward declarations
class AstProgramLine;
class AstDeclartion;
class AstIntalisations;
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
    Token* token;
    bool containsCode();
public:
    AstNode(Token* token);
    AstNode();
    std::string getTokenType();
    std::string getTokenText();
    std::string getJsCode();
};

class AstProgram: AstNode{
    std::vector<AstNode*> lines;
public:
    AstProgram();
    void addLine(AstNode* programLine);
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
class AstDeclartion: AstNode{
    AstVar* var = nullptr;
    AstValue* value = nullptr;
public:
    AstDeclartion(Token* token);
    AstDeclartion(Token* token, AstVar* var, AstValue* value);
    void setAstVar(AstVar* var);
    void setAstValue(AstValue* value);
    AstVar* getAstVar();
    AstValue* getAstValue();
    std::vector<AstNode*> getChilderen();
    std::string getJsCode();
};

class AstIntalisations: AstNode{
    AstVar* var = nullptr;
    AstValue* value = nullptr;
public:
    AstIntalisations();
    AstIntalisations(AstVar* var, AstValue* value);
    void setAstVar(AstVar* var);
    void setAstValue(AstValue* value);
    AstVar* getAstVar();
    AstValue* getAstValue();
    std::vector<AstNode*> getChilderen();
    std::string getJsCode();
};

class AstConditionBody: AstNode{
    AstCondition* condition = nullptr;
    AstBody* value = nullptr;
public:
    AstConditionBody();
    AstConditionBody(AstCondition* condition, AstBody* body);
    void setAstCondition(AstCondition* condition);
    void setAstBody(AstBody* body);
    AstCondition* getAstCondition();
    AstBody* getAstBody();
    std::vector<AstNode*> getChilderen();
    std::string getJsCode();
};

class AstWhile: AstConditionBody{

};

class AstIf: AstConditionBody{

};

class AstBody: AstProgram{

};

class AstCondition: AstNode{
    AstVarOrValue* val1;
    AstVarOrValue* val2;
public:
    AstCondition(Token* token);
    AstCondition(Token* token, AstVarOrValue* var1, AstVarOrValue* var2);
    void setVar1(AstVarOrValue* valOrValue);
    void setVar2(AstVarOrValue* valOrValue);
    std::vector<AstNode*> getChilderen();
    std::string getJsCode();
};

class AstArithmeticOperations: AstNode{
    AstNode* val1;
    AstNode* val2;
public:
    AstArithmeticOperations(Token* token);
    AstArithmeticOperations(Token* token, AstNode* val1, AstNode* val2);
    void setVal1(AstVarOrValue* valOrValue);
    void setVal2(AstVarOrValue* valOrValue);
    std::vector<AstNode*> getChilderen();
    std::string getJsCode();
};

class AstBoolOperator: AstNode{
public:
    AstBoolOperator(Token* token);
    std::string getJsCode();
};

class AstVarOrValue: AstNode{

};

class AstVar: AstVarOrValue{
    AstVar(Token* token);
    std::string getJsCode();
};

class AstValue: AstVarOrValue{
    AstValue(Token* token);
    std::string getJsCode();
};
#endif //TRANSPILER_ASTNODE_H
