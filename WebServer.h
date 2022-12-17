//
// Created by stein on 17/12/2022.
//

#ifndef TRANSPILER_WEBSERVER_H
#define TRANSPILER_WEBSERVER_H
#include "crow.h"
#include "AstNode.h"
#include "tokenizer/ENFA.h"
#include "tokenizer/Tokenizer.h"
#include "CSTNode.h"


class WebServer {
    int port;
public:
    WebServer(int port = 18080);
    void start();
};


#endif //TRANSPILER_WEBSERVER_H
