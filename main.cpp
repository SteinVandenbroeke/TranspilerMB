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

ENFA createENFA(const std::vector<std::string>& words){
    ENFA temp = ENFA();
    std::map<std::string, ENFA_State*> tempStates;
    std::vector<ENFA_State*> tempFinal;
    std::set<char> alfabet;
    auto* start = new ENFA_State("start", true, false);
    tempStates[start->getName()] = start;
    int name = 0;
    for(auto w : words){
        ENFA_State* current = start;
        for(auto c : w){
            alfabet.insert(c);
            auto* next = new ENFA_State(std::to_string(name), false, false);
            if(c == w.back()){
                next->setAccepting(true);
                tempFinal.push_back(next);
            }
            tempStates[next->getName()] = next;
            current->setNextState(c, next);
            current = next;
        }
    }
    temp.setStates(tempStates);
    temp.setCurrentStates({start});
    temp.setFinalStates(tempFinal);
    temp.setStartState(start);
    std::vector<char> alphabet(alfabet.begin(), alfabet.end());
    temp.setAlfabet(alphabet);
    for(const auto& w : words){
        if(!temp.accepts(w)){
            std::cout << "you fucked up" << std::endl;
        }
    }
    return temp;
}

int main() {
    //Tokenizer testing
    //Tokenizer t = Tokenizer();
    //t.convert("inputs/STaalCode.txt");
    std::vector<std::string> variables = {"int", "double", "bool", "string"};
    std::vector<std::string> keywords = {"while", "if", "else", "print"};
    ENFA temp = createENFA(keywords);
    for(auto w : keywords){
        std::cout << temp.accepts(w) << std::endl;
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

    cstConstructionTest();

    return 0;
}

