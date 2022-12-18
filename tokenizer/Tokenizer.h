//
// Created by jens on 12/12/22.
//

#include <vector>
#include <set>
#include <map>
#include <string>
#include <iostream>
#include <fstream>

#include "ENFA.h"
#include "Token.h"

#ifndef TRANSPILER_TOKENIZER_H
#define TRANSPILER_TOKENIZER_H

class Tokenizer {
private:
    std::vector<std::string> separators = {" ", ";", "(", ")", "=", "!=", "<=", ">=", "<", ">", "==", "{", "}", "+", "-", "*", "/"};
    std::vector<std::string> keywords = {";", "print", "int", "double", "char", "string", "=", "while", "if", "(", ")", "<=", ">=", "<", ">", "==", "{", "}", "+", "-", "*", "/"};
    std::map<std::string, std::string> types;
    ENFA* symbols = nullptr;
    ENFA* var = nullptr;
    std::map<int, std::vector<std::string>> split;
    std::vector<Token*> tokens;
public:
    Tokenizer();

    /**
     * construct tokenizer from json file
     * @param json location of json file
     */
    Tokenizer(std::string json);

    ~Tokenizer();

    /**
     *
     * finds separator in line
     *
     * @param lines contains all lines read
     * @param lineNum line we want to read on
     * @param linePos position on line where we want to start reading
     * @return position where program will resume reading
     * @return  string the separator found
     */
    std::pair<int, std::string> findSeparator(std::vector<std::string> lines, int lineNum, int linePos);
    /**
     *
     * finds string/char in line
     *
     * @param lines contains all lines read
     * @param lineNum line we want to read on
     * @param linePos position on line where we want to start reading
     * @return position where program will resume reading
     * @return  string the string/char found
     */
    std::pair<int, std::string> read(std::vector<std::string> lines, int lineNum, int linePos);

    /**
     *
     * @param file location of STaalCode file to split
     * reads lines and separates every "word"
     */
    void readAndSplit(const std::string& file);

    /**
     * takes all words found and checks if they are of a certain type with ENFAs
     * converts into appropriate token
     */
    void constructTokens();

    /**
     *
     * @return vector of tokens
     */
    std::vector<Token*> getTokens();

    /**
     *
     * @param file location of STaalCode file we want to convert into tokens
     * @return vector of tokens
     */
    std::vector<Token*> convert(const std::string& file);

    /**
     * displays all words found
     */
    void displayWords();

    /**
     * displays all tokens found
     */
    void displayTokens();

    const std::vector<std::string> &getSeparators() const;
    void setSeparators(const std::vector<std::string> &separators);

    const std::vector<std::string> &getKeywords() const;
    void setKeywords(const std::vector<std::string> &keywords);

    void addType(std::string typeIdentifier, std::string type);

    void constructVar(std::string json);
};


#endif //TRANSPILER_TOKENIZER_H
