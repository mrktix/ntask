#include "Workspace.h"

Workspace::Workspace(std::filesystem::path norg_ws_path, std::string header) {
    this->block_header = header;
    this->norg_workspace = norg_ws_path;
}

std::vector<Workspace::Block>* Workspace::get_blocks() {
    return &block_list;
}

void Workspace::ParseWorkspace() {
    block_list.clear();

    // loop through the workspace and parse files
    for (const std::filesystem::path norg_folder
            : std::filesystem::directory_iterator(norg_workspace)) {

        if (!std::filesystem::is_directory(norg_folder)) continue; // only folders
        if (norg_folder.filename().string()[0] == '.') continue; // skip dotfiles

        // loop through the files in the folders
        for (const std::filesystem::path norg_file
                : std::filesystem::directory_iterator(norg_folder)) {

            if (!std::filesystem::is_regular_file(norg_file)) continue; // only files
            if (norg_file.extension().string() != "norg") continue; // only norgfiles

            ParseFile(norg_file); // parse the files
        }
    }
}

// private
void Workspace::ParseFile (std::filesystem::path norg_file) {
    // parse thru the file and write to block_list
    std::ifstream file(norg_file);
    if (!file.is_open())
        throw std::runtime_error("unable to open norg file: " + norg_file.string());

    enum stage {
        stage_space, stage_block,
    } parsing_stage = stage_space;

    Workspace::Block current_block;
    current_block.source_file = norg_file;

    while(file) {
        std::string line; std::getline(file, line); boost::algorithm::trim(line);

        if (line == "@code " + block_header) {
            // start of a block, reset the block, enter reading mode
            parsing_stage = stage_block;
            current_block.content = "";
        } else if (line == "@end") {
            // end of a block, push current one to vector and enter passive mode
            parsing_stage = stage_space;
            block_list.push_back(current_block);
        } else if (parsing_stage == stage_block) {
            // add the line in the block to the block content field
            if (current_block.content.empty()) {
                current_block.content = line;
            } else {
                current_block.content += " " + line;
            }
        }
    }
    
    file.close();
}
