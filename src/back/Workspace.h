#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include <boost/algorithm/string/trim.hpp>

class Workspace {
    public:
        Workspace(std::filesystem::path norg_ws_path, std::string header);

        struct Block {
            std::string content;
            std::filesystem::path source_file;
        };

        // header is the "rust task" in "@code rust task ... @end"
        void ParseWorkspace();
        std::vector<Block>* get_blocks();

    private:
        std::filesystem::path norg_workspace;
        std::vector<Block> block_list;
        std::string block_header;

        void ParseFile(std::filesystem::path norg_file);
};
