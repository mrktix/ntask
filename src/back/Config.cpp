#include "Config.h"

Config::Config() {}

void Config::Read() {
    std::string config_str = getenv("XDG_CONFIG_HOME");
    config_str += "/ntask/config";
    std::filesystem::path config_file = config_str;

    Read(config_file);
}

void Config::Read(std::filesystem::path config_file) {
    std::ifstream file(config_file);
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
                ("failed to parse"+config_file.string()+" : "+std::to_string(line_number)+" '"+line+"'");
        }
    }

    file.close();
}

bool Config::ParseLine(std::string line) {
    // skip the line if empty or if it is a comment
    if (line == "" || line[0] == '#') return true;

    enum stage { stage_type, stage_name, stage_value } parsing_stage = stage_type;

    std::string type;
    std::string name;
    std::string value;
    
    // this assumes that there is only one space between the 
    for (int i = 0; i < line.length(); i++) {
        switch (parsing_stage) {
            case stage_type:
                if (line[i] == ' ') parsing_stage = stage_name;
                else type += line[i];
                break;

            case stage_name:
                if (line[i] == ' ') parsing_stage = stage_value;
                else name += line[i];
                break;

            case stage_value:
                if (line[i] == ' ') parsing_stage = stage_value;
                else value += line[i];
                break;
        }
    }

    // make sure that all the variables were filled
    assert(!type.empty());
    assert(!name.empty());
    assert(!value.empty());

    // depending on the var type, populate the correct hashmap with the value
    if (type == "path") {
        if (value[0] == '~')
            value = getenv("HOME") + value.substr(1);
        paths[name] = (std::filesystem::path) value;
    } else if (type == "int") {
        ints[name] = std::stoi(value);
    } else if (type == "str") {
        strs[name] = value;
    } else {
        return false;
    }

    return true;
}

std::string Config::get_str(std::string key) const {
    return strs.find(key)->second;
}

std::filesystem::path Config::get_path(std::string key) const {
    return paths.find(key)->second;
}

int Config::get_int(std::string key) const {
    return ints.find(key)->second;
}
