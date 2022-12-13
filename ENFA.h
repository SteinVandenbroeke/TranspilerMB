//
// Created by jens on 8/12/22.
//

#ifndef TRANSPILER_ENFA_H
#define TRANSPILER_ENFA_H

#include <vector>
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <map>

#include "json.hpp"


class ENFA_State {
private:
    std::string name;
    std::map<char, std::vector<ENFA_State *>> nextState; // map met key: symbool uit alfabet en slaat vector met next states op
    bool starting = false;
    bool accepting = false;
public:
    ENFA_State(){}

    ENFA_State(std::string n){
        name = n;
    }

    ENFA_State(bool starts, bool accepts){
        starting = starts; accepting = accepts;
    }

    ENFA_State(std::string n, bool starts, bool accepts){
        name = n; starting = starts; accepting = accepts;
    }

    const std::string &getName() const;
    void setName(const std::string &name);

    std::vector<ENFA_State*>getNextState(const char s);
    void setNextState(const char s, ENFA_State *newState);

    bool isStarting() const;
    void setStarting(bool starting);

    bool isAccepting() const;
    void setAccepting(bool accepting);
};


class ENFA {
private:
    char eps = '$';
    std::vector<char> alfabet = {eps};
    ENFA_State* startState;
    std::map<std::string, ENFA_State*> states;
    std::vector<ENFA_State*> finalStates;
    std::vector<ENFA_State*> currentStates;
public:
    ENFA(std::string jsonFile);

    ENFA() {}

    ~ENFA(){
        for(auto i : states){ // ga door alle states van de enfa en verwijder ze
            delete i.second;
        }
    }

    nlohmann::json toJSON() const;

    void print();

    std::vector<ENFA_State*> closure(ENFA_State* state);

    const bool accepts(std::string input);

    const std::vector<char> &getAlfabet() const;
    void setAlfabet(const std::vector<char> &alfabet);

    ENFA_State *getStartState() const;
    void setStartState(ENFA_State *startState);

    const std::map<std::string, ENFA_State *> &getStates() const;
    void setStates(const std::map<std::string, ENFA_State *> &states);

    const std::vector<ENFA_State *> &getFinalStates() const;
    void setFinalStates(const std::vector<ENFA_State *> &finalStates);

    const std::vector<ENFA_State *> &getCurrentStates() const;
    void setCurrentStates(const std::vector<ENFA_State *> &currentStates);
};


#endif //TRANSPILER_ENFA_H
