#include "Task.h"

Task::Task(std::string definition, std::filesystem::path source_file) {
    this->source_file = source_file;
    name = "";
    tag = "";
    done = false;

    std::string end_date_str = "";
    std::string done_date_str = "";

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
                else end_date_str += ch;
                break;

            // check if the definition has ended, otherwise add to variable value
            case stage_done_date:
                if (ch == ']') parsing_stage = stage_space;
                else done_date_str += ch;
                break;
        }
    }

    // depending on whether task is done or not, parse out the date
    if (done_date_str.empty()) {
        // task is not done yet
        done = false;
        date = Timestamp(end_date_str);
    } else {
        // task is done
        done = true;
        date = Timestamp(done_date_str);
    }
}

Timestamp Task::get_date() const { return date; }

std::string Task::get_name() const { return name; }

std::string Task::get_tag() const { return tag; }

std::string Task::get_folder() const { 
    return source_file.parent_path().filename().string();
}

std::string Task::get_file() const { return source_file.stem().string(); }

bool Task::is_done() const { return done; }

void Task::Complete() {

}

void Task::Write() const {

}

void Task::LineNumber() const {
    std::ifstream source_file_stream;
    source_file_stream.open(source_file);
    if (!source_file_stream.is_open())
        throw runtime_error("unable to open source file : " + source_file.string());

    enum { stage_space, stage_task } parsing_stage = stage_space;
    bool current_task;

    while (source_file_stream) {
        std::string line; getline(source_file_stream, line); boost::algorithm::trim(line);

        if (parsing_stage == stage_space) {
            if (line == "@code rust task") { // check for start of a task block
                parsing_stage = stage_task;
                current_task = true;
                continue;
            }
            // this line is ignored
        } else { // parsing_stage == stage_task
            if (line == "@end") { // check if it is the end of the bloc
                if (current_task) { // no conflicts between definition and this task

                }
                parsing_stage = stage_space; continue;
            }

            if (line.find("\"") != -1) { // this line specifies name
                if (line.find("\""+name+"\"") == -1) { // task name does not match
                    

                }

            }
        }
    }

    source_file_stream.close();
}
