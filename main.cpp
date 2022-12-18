#include <iostream>
#include "filesystem"
#include "AstNode.h"
#include "tokenizer/ENFA.h"
#include "tokenizer/Tokenizer.h"
#include "CSTNode.h"
#include "WebInterface/WebServer.h"

int main(int argc, char *argv[]) {
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