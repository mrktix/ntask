#pragma once

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include <boost/algorithm/string/trim.hpp>

class Config {
    public:
        Config();

        void Read();
        void Read(std::filesystem::path config_file);

        std::string get_str(std::string key) const;
        std::filesystem::path get_path(std::string key) const;
        int get_int(std::string key) const;

    private:
        bool ParseLine(std::string line);
        std::unordered_map<std::string, std::string> strs;
        std::unordered_map<std::string, std::filesystem::path> paths;
        std::unordered_map<std::string, int> ints;
}
