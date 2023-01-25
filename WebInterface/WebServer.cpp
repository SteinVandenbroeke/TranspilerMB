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

    CROW_ROUTE(app, "/ast.png")
            ([](const crow::request&, crow::response& res) {
                res.set_static_file_info("testEnv/ast.png");
                res.end();
            });

    CROW_ROUTE(app, "/cst.png")
            ([](const crow::request&, crow::response& res) {
                res.set_static_file_info("testEnv/cst.png");
                res.end();
            });

    CROW_ROUTE(app, "/staalCode")
            .methods("GET"_method, "POST"_method)([](const crow::request& req) {

                Tokenizer t = Tokenizer("inputs/tokenizer.json");
                std::ofstream myfile ("staalWebInt.c4");
                myfile << req.body;
                myfile.close();

                std::string errors = "Successfully transpiled!";
                std::string jsCode = "fout";
                try{
                    std::vector<Token*> tokens = t.convert("staalWebInt.c4");
                    CST cst = CST(tokens,"inputs/staal_fixed.json_PARSETABLE.json");
                    std::string cstDot = cst.generateDOT();
                    AstProgram* program = cst.toAst();
                    std::string astDot = program->generateDOT();

                    std::ofstream astFile ("ast.dot");
                    astFile << astDot;
                    astFile.close();

                    system("dot -Tpng ast.dot > testEnv/ast.png");

                    std::ofstream cstFile ("cst.dot");
                    cstFile << cstDot;
                    cstFile.close();

                    system("dot -Tpng cst.dot > testEnv/cst.png");

                    std::stringstream errorStream = std::stringstream();
                    if(program->checkTypes(errorStream)){
                        jsCode = program->getJsCode();
                    }
                    if(errorStream.str() != ""){
                        errors = errorStream.str();
                    }
                }
                catch (std::runtime_error& error){
                    std::cout << error.what() << std::endl;
                    errors = error.what();
                }
                crow::json::wvalue returnValue({{"jscode", jsCode}, {"errors", errors}, {"cst", "cst.png"}, {"ast", "ast.png"}});
                return returnValue;
            });

    app.port(this->port).multithreaded().run();
}