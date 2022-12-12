#include <iostream>
#include "filesystem"
#include "AstNode.h"
#include "ENFA.h"
#include "Tokenizer.h"

int main() {
    //Tokenizer testing
    Tokenizer t = Tokenizer();
    t.convert("inputs/STaalCode.txt");

    //ENFA testing
    /*
    ENFA e = ENFA("inputs/input1.json");
    e.print();
    for(auto s : e.closure(e.getStartState())){
        std::cout << s->getName() << std::endl;
    }
    std::cout << std::endl << e.accepts("0.0") << std::endl;
    */

    /*
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
     */
    return 0;
}