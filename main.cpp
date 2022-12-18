#include <iostream>
#include "filesystem"
#include "AstNode.h"
#include "tokenizer/ENFA.h"
#include "tokenizer/Tokenizer.h"
#include "CSTNode.h"
#include "WebServer.h"


void cstConstructionTest() {
    std::vector<Token> tokens {Token{"c"}, Token{"c"}, Token{"d"}, Token{"d"}, Token{"$"}};
    std::vector<Token*> tokenptrs = {&tokens[0],&tokens[1],&tokens[2], &tokens[3], &tokens[4]};
    try{
        CST cst{tokenptrs, "docs/test_grammar_0_PARSETABLE.json"};
    } catch (const std::runtime_error& e){
        std::cerr << e.what() << std::endl;
    }
}

int main(int argc, char *argv[]) {
    if(argc == 1 || argv[1] == "-webInterface"){
        WebServer ws = WebServer();
        ws.start();
    }
    else if(argc == 2){
        Tokenizer t = Tokenizer();
        std::vector<Token*> tokens = t.convert(argv[1]);
        CST cst = CST(tokens,"LR1ParserGenerator/test_grammar_0.json_PARSETABLE.json");
        // std::cout << cst.generateDOT() << std::endl;
        AstProgram* program = cst.toAst();
        std::stringstream errorStream = std::stringstream();
        if(program->checkTypes(errorStream)){
            std::cout << errorStream.str();
            std::cout << program->getJsCode() << std::endl;
        }
        else{
            std::cout << errorStream.str();
        }
    }
    return 0;
}