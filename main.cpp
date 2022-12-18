#include <iostream>
#include "filesystem"
#include "AstNode.h"
#include "tokenizer/ENFA.h"
#include "tokenizer/Tokenizer.h"
#include "CSTNode.h"
#include "WebInterface/WebServer.h"

/*TODO nog nodig?
void cstConstructionTest() {
    std::vector<Token> tokens {Token{"c"}, Token{"c"}, Token{"d"}, Token{"d"}, Token{"$"}};
    std::vector<Token*> tokenptrs = {&tokens[0],&tokens[1],&tokens[2], &tokens[3], &tokens[4]};
    try{
        CST cst{tokenptrs, "docs/test_grammar_0_PARSETABLE.json"};
    } catch (const std::runtime_error& e){
        std::cerr << e.what() << std::endl;
    }
}
*/
void tokenizerTest(){
    Tokenizer t = Tokenizer("inputs/tokenizer.json");
    t.convert("inputs/STaalCode.txt");
    t.displayWords();
    t.displayTokens();
}

int main(int argc, char *argv[]) {
    //tokenizerTest();

    if(argc == 3){
        Tokenizer t = Tokenizer("inputs/tokenizer.json");
        std::vector<Token*> tokens = t.convert(argv[1]);
        CST cst = CST(tokens,"LR1ParserGenerator/test_grammar_0.json_PARSETABLE.json");
        // std::cout << cst.generateDOT() << std::endl;
        AstProgram* program = cst.toAst();
        std::stringstream errorStream = std::stringstream();
        bool typesOk = program->checkTypes(errorStream);
        std::cout << errorStream.str();
        if(typesOk){
            std::ofstream myfile (argv[2]);
            myfile << program->getJsCode();
            myfile.close();
            std::cout << "transpiling done" << std::endl;
        }
        delete program;
    }
    else{
        std::cout << "Wrong parameter count" << std::endl;
        std::cout << "call with \"./transpiler [staal file] [export file]\"" << std::endl;
    }
    return 0;
}