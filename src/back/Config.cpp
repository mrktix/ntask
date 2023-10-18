#include "Config.h"


Config::Config() {

}

void Config::Read() const {
    std::string config_str = getenv("XDG_CONFIG_HOME");
    config_str += "/ntask/config"
    std::filesystem::path config_file = config_str;

    Read(config_file);
}

void Config::Read(std::filesystem::path config_file) {
    std::ifstream file;
    file.open(config_file);
    if (!file.is_open())
        throw std::runtime_error("unable to open config file: " + config_file.string());

    std::string error;
    int line_number = 0;
    
    while(file) {
        line_number++;
        std::string line;

        std::getline(file, line);
        boost::algorithm::trim(line);

        // parse the line and throw error on failure
        if (!ParseLine(line)) {
            file.close();
            throw std::runtime_error
                ("failed to parse"+config_file.string()+":"+line_number+" '"+line+"'");
        }
    }

    file.close();
}

bool Config::ParseLine(std::string line) {
    if (line == "" || line[0] == '#') return true; // skip the line if empty or if it is a comment

    enum stage { stage_type, stage_name, stage_value } parsing_stage = stage_type;

    std::string type;
    std::string name;
    std::string value;
    
    for (int i = 0; i < line.length(); i++) {
        switch (parsing_stage) {
            case stage_type:
                if (line[i] == ' ') parsing_stage = stage_name;
                else type += line[i];
                break;

            case stage_name:
                if (line[i] == ' ') parsing_stage = stage_value;
                else type += line[i];
                break;

            case stage_value:
                if (line[i] == ' ') parsing_stage = stage_value;
                else type += line[i];
                break;
        }
    }
}
