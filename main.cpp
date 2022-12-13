#include <iostream>
#include "filesystem"
#include "AstNode.h"
#include "ENFA.h"
#include "Tokenizer.h"

#include "CSTNode.h"


int cstConstructionTest() {
    std::vector<Token> tokens {Token{"c"}, Token{"d"}, Token{"d"}, Token{"$"}};
    std::vector<Token*> tokenptrs = {&tokens[0],&tokens[1],&tokens[2]};
    CST cst{tokenptrs, "LR1ParserGenerator/test_grammar_0.json_PARSETABLE.json"};
}

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

    //cstConstructionTest();

    return 0;
}

