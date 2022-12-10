//
// Created by jens on 8/12/22.
//

#include "ENFA.h"

//ENFA_State
const std::string &ENFA_State::getName() const {
    return name;
}

void ENFA_State::setName(const std::string &name) {
    ENFA_State::name = name;
    return;
}

std::vector<ENFA_State *> ENFA_State::getNextState(const char s){
    if(nextState.find(s) == nextState.end()){
        return {};
    }
    return ENFA_State::nextState[s];
}

void ENFA_State::setNextState(const char s, ENFA_State *newState) {
    nextState[s].push_back(newState);
}

bool ENFA_State::isStarting() const {
    return starting;
}

void ENFA_State::setStarting(bool starting) {
    ENFA_State::starting = starting;
}

bool ENFA_State::isAccepting() const {
    return accepting;
}

void ENFA_State::setAccepting(bool accepting) {
    ENFA_State::accepting = accepting;
}

//ENFA

ENFA::ENFA(std::string jsonFile) {
    std::ifstream input(jsonFile); // lees de inputfile
    nlohmann::json j;
    input >> j;
    if(j["type"] != "ENFA"){ // check of het een NFA is
        return;
    }
    for(auto i : j["alphabet"]){ // ga naar het segment alphabet in de json file en ga doorheen de elementen
        std::string temp = to_string(i); // convert het element naar een string
        alfabet.push_back(temp[1]); // neem het middelste element van de nieuwe string
    }
    for(auto k : j["states"]){ // ga naar het segment states in de json file en ga doorheen de elementen
        ENFA_State* s = new ENFA_State(k["name"]);  // maak een nieuwe state met naam van de huidige state
        states[k["name"]] = s; // voeg nieuwe state toe aan map met alle states
        if(k["starting"] == true){ // check of het de startingstate is
            s->setStarting(true);
            currentStates.push_back(s);
            startState = s; // zet de current en starting state op de huidige state
        }
        if(k["accepting"] == true){ // check of het een acceptingstate is
            s->setAccepting(true);
            finalStates.push_back(s); // voeg huidige state toe aan finalstates
        }
    }
    for(auto t : j["transitions"]){ // doorloop de transitions in de json file
        std::string input = to_string(t["input"]);
        char in;
        if(input.size() > 2){
            in = input[1];
        }else{
            in = eps;
        }
        states[t["from"]]->setNextState(in, states[t["to"]]); // access de benodigde states uit map
    }
}

nlohmann::json ENFA::toJSON() const {
    nlohmann::json j;
    j["type"] = "ENFA";
    j["alphabet"] = {};
    for(auto symbol : alfabet) {
        j["alphabet"].push_back(std::string{symbol});
    }

    j["states"] = {};
    j["transitions"] = {};
    int a = 0; int b = 0;
    for(const auto& s : states) {
        j["states"].push_back({});
        j["states"][a]["name"] = s.first;
        j["states"][a]["starting"] = s.second->isStarting();
        j["states"][a]["accepting"] = s.second->isAccepting();
        a++;
        for(const auto chr : alfabet) {
            for(const auto& toState : s.second->getNextState(chr)) {
                j["transitions"].push_back({});
                j["transitions"][b]["from"] = s.first;
                j["transitions"][b]["to"] = toState->getName();
                std::string input = "";
                if(chr != eps){
                    input = std::string{chr};
                }
                j["transitions"][b]["input"] = input;
                b++;
            }
        }
    }
    return j;
}

void ENFA::print() {
    std::cout << std::setw(4) << toJSON() << std::endl;
}

std::vector<ENFA_State*> ENFA::closure(ENFA_State* state){
    std::set<ENFA_State*> result = {state};
    int oldSize = result.size();
    std::vector<ENFA_State*> temp = state->getNextState(eps);
    result.insert(temp.begin(), temp.end());
    while(result.size() > oldSize){
        oldSize = result.size();
        for(auto i : result){
            temp = i->getNextState(eps);
            result.insert(temp.begin(), temp.end());
        }
    }
    return std::vector(result.begin(), result.end());

}

const bool ENFA::accepts(std::string input) {
    // ga door elk element van de string
    currentStates = {startState};
    std::set current = {startState};
    for (auto i : input){
        if(count(alfabet.begin(), alfabet.end(), i) < 1){
            return false;
        }
        std::vector<ENFA_State*> temp;
        std::vector<ENFA_State*> close;
        std::set<ENFA_State*> temp0;
        std::set<ENFA_State*> temp1;
        for(auto s : currentStates){
            close = this->closure(s);
            current.insert(close.begin(), close.end());
        }
        std::vector<ENFA_State*> temp2(current.begin(), current.end());
        currentStates = temp2;

        for(auto s : currentStates){
            temp = s->getNextState(i); // bepaal de nextstates
            temp1 = {};
            temp1.insert(temp.begin(), temp.end());
            temp0.insert(currentStates.begin(), currentStates.end());
            temp0.insert(temp1.begin(), temp1.end());
        }
        std::vector<ENFA_State*> temp3(temp0.begin(), temp0.end());
        currentStates = temp3;

    }
    for(auto state : currentStates){
        if(state->isAccepting()){ // check of de currentstate een finalstates is
            currentStates = {startState};
            return true;
        }
    }
    currentStates = {startState};
    return false;
}

const std::vector<char> &ENFA::getAlfabet() const {
    return alfabet;
}

void ENFA::setAlfabet(const std::vector<char> &alfabet) {
    ENFA::alfabet = alfabet;
}

ENFA_State *ENFA::getStartState() const {
    return startState;
}

void ENFA::setStartState(ENFA_State *startState) {
    ENFA::startState = startState;
}

const std::map<std::string, ENFA_State *> &ENFA::getStates() const {
    return states;
}

void ENFA::setStates(const std::map<std::string, ENFA_State *> &states) {
    ENFA::states = states;
}

const std::vector<ENFA_State *> &ENFA::getFinalStates() const {
    return finalStates;
}

void ENFA::setFinalStates(const std::vector<ENFA_State *> &finalStates) {
    ENFA::finalStates = finalStates;
}

const std::vector<ENFA_State *> &ENFA::getCurrentStates() const {
    return currentStates;
}

void ENFA::setCurrentStates(const std::vector<ENFA_State *> &currentStates) {
    ENFA::currentStates = currentStates;
}