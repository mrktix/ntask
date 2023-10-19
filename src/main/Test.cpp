#include "Test.h"

int main(int argc, char** argv) {
    int total_fail = 0;

    std::cout << std::endl;
    int config_fail = ConfigTest();
    std::cout << "ConfigTest fail count: " << std::to_string(config_fail) << std::endl;
    total_fail += config_fail;
    
    std::cout << std::endl << "Total test fail count: " << std::to_string(total_fail) << std::endl;

    return 0;
}

void make_sure(bool condition, int& ref_fail) {
    if (condition) {
        std::cout << "success" << std::endl;
    } else {
        ref_fail++;
        std::cout << "success" << std::endl;
    }
    std::cout << std::endl;
}

int ConfigTest() {
    Config config;

    int fails = 0;
    
    std::cout << "Config.Read(\"/home/arleok/.config/ntask/default\")" << std::endl;
    try {
        config.Read((std::filesystem::path) "/home/arleok/.config/ntask/default");
    } catch (...) {
        make_sure(false, fails);
        return fails;
    }
    make_sure(true, fails);

    std::string str = config.get_str("username");
    std::cout << "Config.get_str(\"username\") = " << str << std::endl;
    make_sure(str == "arleok", fails);

    int integer = config.get_int("color1");
    std::cout << "Config.get_int(\"color1\") = " << integer << std::endl;
    make_sure(integer == 5, fails);

    std::string path = config.get_path("workspace").string();
    std::cout << "Config.get_path(\"workspace\") = " << path << std::endl;
    make_sure(path == "/home/arleok/wiki/", fails);

    return fails;
}
