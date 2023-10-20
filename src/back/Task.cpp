#include "Task.h"

long Task::future_date = 65000000000;

Task::Task() {}

void Task::Read(std::string definition, std::filesystem::path source_file) {
    this->source_file = source_file;
    name = "";
    tag = "";
    done = false;
    date = 0;

    std::string end_date_parse = "";
    std::string done_date_parse = "";

    enum stage {
        stage_space, stage_name, stage_tag, stage_end_date, stage_done_date
    } parsing_stage = stage_space;

    // parse out the separate variable definitions into their variables (as strings)
    for (int i = 0; i < definition.length(); i++) {
        char ch = definition[i];

        switch (parsing_stage) {
            // check for start of a variable definition, otherwise move on
            case stage_space:
                switch (ch) {
                    case '"': parsing_stage = stage_name; break;
                    case '<': parsing_stage = stage_tag; break;
                    case '{': parsing_stage = stage_end_date; break;
                    case '[': parsing_stage = stage_done_date; break;
                }
                break;

            // check if the definition has ended, otherwise add to variable value
            case stage_name:
                if (ch == '"') parsing_stage = stage_space;
                else name += ch;
                break;

            // check if the definition has ended, otherwise add to variable value
            case stage_tag:
                if (ch == '>') parsing_stage = stage_space;
                else tag += ch;
                break;

            // check if the definition has ended, otherwise add to variable value
            case stage_end_date:
                if (ch == '}') parsing_stage = stage_space;
                else end_date_parse += ch;
                break;

            // check if the definition has ended, otherwise add to variable value
            case stage_done_date:
                if (ch == ']') parsing_stage = stage_space;
                else done_date_parse += ch;
                break;
        }
    }

    // 
}
