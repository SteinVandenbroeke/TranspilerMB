#include <iostream>
#include "filesystem"

int main() {
    std::cout << "Transpiling" << std::endl;
    std::string testEnv = std::string ("xdg-open \"") + std::string (std::filesystem::current_path()) + "/testEnv/index.html\"";
    std::cout << testEnv << std::endl;
    system(testEnv.c_str());
    return 0;
}