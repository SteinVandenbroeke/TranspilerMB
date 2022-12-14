#include <iostream>
#include "filesystem"
#include "AstNode.h"
#include "tokenizer/ENFA.h"
#include "tokenizer/Tokenizer.h"

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
    /*
    Tokenizer t = Tokenizer();
    std::vector<Token*> tokens = t.convert("inputs/STaalCode.txt");
    CST cst = CST(tokens,"LR1ParserGenerator/test_grammar_0.json_PARSETABLE.json");
    std::cout << cst.generateDOT() << std::endl;
    */
    //Tokenizer testing

    Tokenizer t0 = Tokenizer();
    std::vector<Token*> tokens0 = t0.convert("inputs/STaalCode.txt");

    for(auto token : tokens0){
        token->print();
    }

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

    Token* tokenVarI = new Token("[VARNAMETOKEN]", "i");
    AstVar* varI = new AstVar(tokenVarI);

    Token* tokenDeclarationA = new Token("int");
    Token* tokenDeclarationI = new Token("int");

    Token* number = new Token("[NUMBERTOKEN]", "10");
    AstValue* value = new AstValue(number);
*/
//    std::vector<Token *> tokenList{new Token("int"),new Token("[VARNAMETOKEN]","i"),new Token("="),new Token("[NUMBERTOKEN]", "10"),new Token(";"),new Token("while"),new Token("("),new Token("[VARNAMETOKEN]","i"), new Token("<"), new Token("[NUMBERTOKEN]","11"),new Token(")"),new Token("{"), new Token("int"),new Token("[VARNAMETOKEN]","a"),new Token("="),new Token("[NUMBERTOKEN]", "10"),new Token(";"),new Token("}"), new Token("$")};
//    CST cst = CST(tokenList,"LR1ParserGenerator/test_grammar_0.json_PARSETABLE.json");
//    cst.generateDOT();
/*
    AstDeclartion* declartionI = new AstDeclartion(tokenDeclarationI, varI, value);
    AstDeclartion* declartionA = new AstDeclartion(tokenDeclarationA, varA, value);

    AstIntalisation* intalisation = new AstIntalisation(varI, varA);


    AstProgram program = AstProgram();
    program.addLine(declartionI);
    program.addLine(declartionA);
    //program.addLine(declartionA);
    //program.addLine(declartion);
    //program.addLine(declartion);
    program.addLine(intalisation);
    program.checkTypes();
    std::cout << program.getJsCode();

*/
   // cstConstructionTest();

    return 0;
}