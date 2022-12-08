#include <iostream>
#include "filesystem"
#include "AstNode.h"

int main() {
    Token* tokenVarI = new Token("[VARNAMETOKEN]", "i");
    AstVar* varI = new AstVar(tokenVarI);

    Token* tokenValue = new Token("[NUMBERTOKEN]", "10");
    AstValue* value = new AstValue(tokenValue);

    Token* tokenDeclaration = new Token("int");
    AstDeclartion* declartion = new AstDeclartion(tokenDeclaration, varI, value);

    AstIntalisation* intalisation = new AstIntalisation(varI, value);

    AstProgram program = AstProgram();
    program.addLine(intalisation);
    program.addLine(declartion);
    program.addLine(declartion);
    //program.addLine(declartion);
    program.addLine(intalisation);
    program.checkTypes();
    std::cout << program.getJsCode();
    return 0;
}