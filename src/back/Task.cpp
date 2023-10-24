#include "Task.h"

Task::Task(std::string definition, std::filesystem::path source_file) {
    this->source_file = source_file;
    name = "";
    tag = "";
    done = false;

    std::string due_date_str = "";
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
                else due_date_str += ch;
                break;

            // check if the definition has ended, otherwise add to variable value
            case stage_done_date:
                if (ch == ']') parsing_stage = stage_space;
                else done_date_str += ch;
                break;
        }
    }

    // depending on whether task is done or not, parse out the date
    date_due = Timestamp(due_date_str);

    if (done_date_str.empty()) {
        done = false;
        date_done = Timestamp(true); //futuredate
    } else {
        done = true;
        date_done = Timestamp(done_date_str);
    }
}

Timestamp Task::get_date() const { return is_done()? date_done : date_due; }
std::string Task::get_name() const { return name; }
std::string Task::get_tag() const { return tag; }
std::string Task::get_file() const { return source_file.stem().string(); }
bool Task::is_done() const { return done; }
std::string Task::get_folder() const { 
    return source_file.parent_path().filename().string();
}

void Task::Complete() {

}

void Task::Write() const {

}

int Task::LineNumber() const {
    std::ifstream source_file_stream;
    source_file_stream.open(source_file);
    if (!source_file_stream.is_open())
        throw runtime_error("unable to open source file : " + source_file.string());

    enum { stage_space, stage_task } parsing_stage = stage_space;
    int line_number = 0;
    bool found_match = false;

    while (source_file_stream) { line_number++;
        // get a line, and trim whitespace on sides
        std::string line; getline(source_file_stream, line); boost::algorithm::trim(line);

        if (parsing_stage == stage_space) {
            // look for the start of a task block
            if (line == "@code rust task") { parsing_stage = stage_task; }
        } else {
            // we got to end without leaving task parsing stage: the task matches
            if (line == "@end") { found_match = true; break; }

            // check for conflicts in definition, if found, ignore rest of task
            if (line.find("\"") != -1 && line.find("\""+name+"\"") == -1) {
                // line contains a name definition but it doesn't match
                parsing_stage = stage_space;
            } else if (!is_done() &&
                    line.find("{") != -1 && line.find("{"+date.get_str()+"}")) {
                // line contains a due date definition but it doesn't match
                parsing_stage = stage_space;
            } else if (is_done() &&
                    line.find("[") != -1 && line.find("["+date.get_str()+"]")) {
                // line contains a done date definition but it doesn't match
                parsing_stage = stage_space;
            }
        }
    }

    source_file_stream.close();

    if (!found_match) { throw runtime_error
        ("could not find task '"+get_name()+"' in file '"+source_file.string()+"'");
    }

    return line_number;
}
