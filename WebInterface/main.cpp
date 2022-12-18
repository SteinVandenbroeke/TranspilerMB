#include <iostream>
#include "filesystem"
#include "../AstNode.h"
#include "../tokenizer/ENFA.h"
#include "../tokenizer/Tokenizer.h"
#include "../CSTNode.h"
#include "WebServer.h"

int main(int argc, char *argv[]) {
    WebServer ws = WebServer();
    ws.start();
    return 0;
}