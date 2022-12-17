//
// Created by stein on 17/12/2022.
//

#include "WebServer.h"

WebServer::WebServer(int port): port(port) {

}


void WebServer::start() {
    crow::SimpleApp app;
    crow::mustache::set_base(".");

    CROW_ROUTE(app, "/")([](){
        crow::mustache::context ctx;
        std::ifstream t("testEnv/index.html");
        std::string indexPage((std::istreambuf_iterator<char>(t)),
                              std::istreambuf_iterator<char>());

        return indexPage;
    });

    CROW_ROUTE(app, "/staalCode")
            .methods("GET"_method, "POST"_method)([](const crow::request& req) {
                Tokenizer t = Tokenizer();
                std::ofstream myfile ("staalWebInt.c4");
                myfile << req.body;
                myfile.close();

                std::vector<Token*> tokens = t.convert("staalWebInt.c4");
                CST cst = CST(tokens,"LR1ParserGenerator/test_grammar_0.json_PARSETABLE.json");
                std::string cstDot = cst.generateDOT();
                AstProgram* program = cst.toAst();
                std::string astDot = program->generateDOT();

                crow::json::wvalue returnValue({{"jscode", program->getJsCode()}, {"errors", ""}, {"cst", cstDot}, {"ast", astDot}});
                return returnValue;
            });

    app.port(this->port).multithreaded().run();
}