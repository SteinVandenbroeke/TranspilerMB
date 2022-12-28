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
                Tokenizer t = Tokenizer("inputs/tokenizer.json");
                std::ofstream myfile ("staalWebInt.c4");
                myfile << req.body;
                myfile.close();

                std::vector<Token*> tokens = t.convert("staalWebInt.c4");
                CST cst = CST(tokens,"inputs/staal_fixed.json_PARSETABLE.json");
                std::string cstDot = cst.generateDOT();
                AstProgram* program = cst.toAst();
                std::string astDot = program->generateDOT();

                std::stringstream errorStream = std::stringstream();
                std::string jsCode = "fout";
                if(program->checkTypes(errorStream)){
                    jsCode = program->getJsCode();
                }
                crow::json::wvalue returnValue({{"jscode", jsCode}, {"errors", errorStream.str()}, {"cst", cstDot}, {"ast", astDot}});
                return returnValue;
            });

    app.port(this->port).multithreaded().run();
}