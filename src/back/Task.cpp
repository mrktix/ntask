#include "Task.h"

std::string Task::field_start_char[4] = { "\"", "{", "[", "<" };
std::string Task::field_end_char[4] = { "\"", "}", "]", ">" };

Task::Task(Workspace::Block definition) {
    Construct(definition.content, definition.source_file);
}

Task::Task(std::string definition, std::filesystem::path source_file) {
    Construct(definition, source_file);
}

// priv
void Task::Construct(std::string definition, std::filesystem::path source_file) {
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
    done = true;
    date_done = Timestamp();
    Write((bool[4]){ false, false, true, false });
}

// priv
void Task::Write(bool replace_which[4]) const {
    // read source_file into text_vec
    std::ifstream file_in(source_file);
    if (!file_in.is_open())
        throw std::runtime_error("unable to open norg file: " + source_file.string());
    std::vector<std::string> text_vec;
    while(file_in) {
        std::string line; std::getline(file_in, line);
        text_vec.push_back(line);
    }
    file_in.close();

    bool replaced_which[4] = { false, false, false, false };
    std::string field_value[4] = { "", "", "", "" };

    for (int i = 0; i < 4; i++) {
        if (!replace_which[i]) continue; // fields that won't be replaced don't matter
        
        switch (i) {
            case 0: field_value[i] = get_name(); break;
            case 1: field_value[i] = date_due.get_str(); break;
            case 2: field_value[i] = date_done.get_str(); break;
            case 3: field_value[i] = get_tag(); break;
        }
    }

    // loop through the definition block, and replace any definitions with current values
    int definition_start_line = LineNumber();
    int definition_end_line = definition_start_line + 1;
    for (int i = definition_start_line; i < text_vec.size(); i++) {
        // check for the end of the definition
        if (boost::algorithm::trim_copy(text_vec[i]) == "@end") {
            definition_end_line = i; break;
        }
        
        // loop thru the 4 fields and replace if necessary
        for (int j = 0; j < 4; j++) {
            if (!replace_which[j] || replaced_which[j])
                continue; // not supposed be to or already has been replaced

            if (text_vec[i].find(field_start_char[j]) == -1 || text_vec[i].find(field_end_char[j]) == -1)
                continue; // this line doesn't define this field

            // replace the definition with the current val
            std::regex match(field_start_char[j] + ".*" + field_end_char[j]);
            std::string replace = field_start_char[j] + field_value[j] + field_end_char[j];
            text_vec[i] = std::regex_replace(text_vec[i], match, replace);
            replaced_which[j] = true;
        }
    }

    // count the spaces at the start of a sample line
    std::string sample_line = text_vec[definition_start_line];
    std::string indent_str = sample_line.substr(0, sample_line.find_first_not_of(' '));

    // if we were meant to write a field, but it has not been written
    // (can occur when the feild was previously undefined)
    // we insert a new line that defines the field
    for (int i = 0; i < 4; i++) {
        if (!replace_which[i] || replaced_which[i])
            continue; // not supposed be to or already has been replaced

        // get the line where you should insert the field
        int insert_index = 0;
        switch (i) {
            case 0: insert_index = definition_start_line+1; break; //name
            case 1: insert_index = definition_end_line;     break; //due
            case 2: insert_index = definition_end_line;     break; //done
            case 3: insert_index = definition_start_line+2; break; //tag
        }

        // insert an indented field definition into the vector
        text_vec.insert(text_vec.begin()+insert_index, 
                indent_str + field_start_char[i] + field_value[i] + field_end_char[i]);
        replaced_which[i] = true;
    }

    // write text_vec to source_file, overriding anything there previously
    std::ofstream file_out(source_file, std::ios::trunc);
    std::string text_string;
    for (std::string text_line : text_vec) {
        text_string += text_line + "\n";
    }
    text_string.pop_back(); // remove trailing newline
    file_out << text_string;
    file_out.close();
}

// priv
int Task::LineNumber() const {
    std::ifstream source_file_stream;
    source_file_stream.open(source_file);
    if (!source_file_stream.is_open())
        throw std::runtime_error("unable to open source file : " + source_file.string());

    enum { stage_space, stage_task } parsing_stage = stage_space;
    
    // false: feild not checked, true: field matches
    bool field_status[4] = { false, false, false, false };
    int line_number = 0;
    int current_block_start_line = 0;

    while (source_file_stream) { line_number++;
        // get a line, and trim whitespace on sides
        std::string line; getline(source_file_stream, line); boost::algorithm::trim(line);

        if (parsing_stage == stage_space) {
            // when we find a task, save starting line and start looking thru it
            if (line == "@code rust task") { 
                parsing_stage = stage_task; 
                current_block_start_line = line_number;
            }
        } else {
            if (line == "@end") { 
                // if any field matched, set any_match to true
                bool any_match = false;
                for (int status : field_status) { if (status) {
                    any_match = true;
                break; }}

                // if there were any matches, return the line number
                if (any_match) {
                    source_file_stream.close();
                    return current_block_start_line;
                }

                // end of a non matching task block, reset status and move to next line
                for (int i = 0; i < 4; i++) field_status[i] = false;
                parsing_stage = stage_space;
                continue;
            }

            // loop thru the fields, and check if this line matches/conflicts
            for (int i = 0; i < 4; i++) {
                std::string start_ch = field_start_char[i], end_ch = field_end_char[i];

                // make sure this line defines field 'i'
                if (line.find(start_ch) == -1 || line.find(end_ch) == -1) continue;

                bool match;
                switch (i) {
                    case 0: // name
                        match = line.find(start_ch+get_name()+end_ch) != -1; break;
                    case 3: // tag
                        match = line.find(start_ch+get_tag()+end_ch) != -1; break;
                    default: // {1,2} due or done (dates)
                        size_t start = line.find(start_ch) + 1;
                        size_t len = line.find(end_ch) - start;
                        match = Timestamp(line.substr(start, len)) == date_due;
                        break;
                }

                if (match) {
                    field_status[i] = true;
                } else {
                    // there is a conflict, stop checking this task
                    parsing_stage = stage_space;
                    break;
                }
            }
        }
    }

    source_file_stream.close();

    // if we got thru file without finding the task, throw an err
    throw std::runtime_error
        ("could not find task '"+get_name()+"' in file '"+source_file.string()+"'");
}

bool Task::operator==(const Task& rhs) const {
    return get_date() == rhs.get_date();
}
bool Task::operator<(const Task& rhs) const {
    return get_date() < rhs.get_date();
}

bool Task::operator!=(const Task& rhs) const { return !(*this == rhs); }
bool Task::operator>=(const Task& rhs) const { return !(*this < rhs); }
bool Task::operator>(const Task& rhs) const { return !(*this < rhs || *this == rhs); }
bool Task::operator<=(const Task& rhs) const { return *this < rhs && *this == rhs; }
