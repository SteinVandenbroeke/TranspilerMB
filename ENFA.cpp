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

void ENFA::transitionHelper(std::string input, std::vector<std::vector<char>> vectors, const std::string& from, const std::string& to) {
    if(input.size() > 2){
        input.pop_back();
        input.erase(input.begin());
    }
    char in;
    if(input[0] == 'v'){
        input.erase(input.begin());
        for(auto c : vectors[std::stoi(input)]){
            states[from]->setNextState(c, states[to]);
        }
        return;
    }
    if(input.empty()){
        in = eps;
    }else{
        in = input[1];
    }
    states[from]->setNextState(in, states[to]); // access de benodigde states uit map
}

ENFA::ENFA(const std::string& jsonFile) {
    std::ifstream input(jsonFile); // lees de inputfile
    nlohmann::json j;
    input >> j;
    std::vector<std::vector<char>> vectors;
    if(j["type"] != "ENFA"){ // check of het een NFA is
        return;
    }
    int vectorCount = 0;
    std::string vector = "v" + std::to_string(vectorCount);
    while(j.contains(vector)){
        std::vector<char> temp;
        for(const auto& i : j[vector]){
            temp.push_back(to_string(i)[1]);
        }
        vectors.push_back(temp);
        vectorCount++;
        vector = "v" + std::to_string(vectorCount);
    }

    for(const auto& i : j["alphabet"]){ // ga naar het segment alphabet in de json file en ga doorheen de elementen
        std::string temp = to_string(i); // convert het element naar een string
        temp.pop_back();
        temp.erase(temp.begin());
        if(temp.size() > 1){
            temp.erase(temp.begin());
            for(auto c : vectors[std::stoi(temp)]){
                alfabet.push_back(c);
            }
            continue;
        }
        alfabet.push_back(temp[0]); // neem het middelste element van de nieuwe string
    }
    for(auto k : j["states"]){ // ga naar het segment states in de json file en ga doorheen de elementen
        auto* s = new ENFA_State(k["name"]);  // maak een nieuwe state met naam van de huidige state
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
        std::string from = t["from"];
        std::string to = t["to"];
        if(input[0] == '['){
            for(const auto& trans : t["input"]){
                transitionHelper(trans, vectors, from, to);
            }
            continue;
        }
        transitionHelper(input, vectors, from, to);
    }
}

ENFA::ENFA(const std::vector<std::string>& words){
    ENFA temp = ENFA();
    std::map<std::string, ENFA_State*> tempStates;
    std::vector<ENFA_State*> tempFinal;
    std::set<char> alfa;
    auto* start = new ENFA_State("start", true, false);
    tempStates[start->getName()] = start;
    int name = 0;
    for(auto w : words){
        ENFA_State* current = start;
        for(auto c : w){
            alfa.insert(c);
            auto* next = new ENFA_State(std::to_string(name), false, false);
            if(c == w.back()){
                next->setAccepting(true);
                tempFinal.push_back(next);
            }
            tempStates[next->getName()] = next;
            current->setNextState(c, next);
            current = next;
            name++;
        }
    }
    states = tempStates;
    currentStates = {start};
    finalStates = tempFinal;
    startState = start;
    std::vector<char> alphabet(alfa.begin(), alfa.end());
    alfabet = alphabet;
    for(const auto& w : words){
        if(!accepts(w)){
            std::cout << "you fucked up" << std::endl;
        }
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
                std::string input;
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

void ENFA::print() const {
    std::cout << std::setw(4) << toJSON() << std::endl;
}

std::vector<ENFA_State*> ENFA::closure(ENFA_State* state) const{
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

bool ENFA::accepts(const std::string& input) {
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