#include <iostream>
#include "filesystem"
#include "AstNode.h"
#include "ENFA.h"
#include "Tokenizer.h"

#include "CSTNode.h"


void cstConstructionTest() {
    std::vector<Token> tokens {Token{"c"}, Token{"c"}, Token{"d"}, Token{"d"}, Token{"$"}};
    std::vector<Token*> tokenptrs = {&tokens[0],&tokens[1],&tokens[2], &tokens[3], &tokens[4]};
    try{
        CST cst{tokenptrs, "docs/test_grammar_0_PARSETABLE.json"};
    } catch (const std::runtime_error& e){
        std::cerr << e.what() << std::endl;
    }
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
    Token* tokenVarA = new Token("[VARNAMETOKEN]", "a");
    AstVar* varA = new AstVar(tokenVarA);

    Token* tokenDeclarationA = new Token("int");

    std::vector<Token *> tokenList{new Token("int"),new Token("[VARNAMETOKEN]","i"),new Token("="),new Token("[NUMBERTOKEN]", "10"),new Token(";"),new Token("while"),new Token("("),new Token("[VARNAMETOKEN]","i"), new Token("<"), new Token("[NUMBERTOKEN]","11"),new Token(")"),new Token("{"), new Token("int"),new Token("[VARNAMETOKEN]","a"),new Token("="),new Token("[NUMBERTOKEN]", "10"),new Token(";"),new Token("}"), new Token("$")};
    CST cst = CST(tokenList,"LR1ParserGenerator/test_grammar_0.json_PARSETABLE.json");


    AstDeclartion* declartionA = new AstDeclartion(tokenDeclarationA, varA, value);

    AstIntalisation* intalisation = new AstIntalisation(varI, varA);


    AstProgram program = AstProgram();
    program.addLine(intalisation);
    program.addLine(declartion);
    program.addLine(declartion);
    //program.addLine(declartion);
    program.addLine(intalisation);
    program.checkTypes();
    std::cout << program.getJsCode();
     */



    cstConstructionTest();

    return 0;
}