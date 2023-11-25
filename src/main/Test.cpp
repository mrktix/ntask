#include "Test.h"

int main(int argc, char** argv) {
    newline();
    int before;

    
    before=fails; ConfigTest("Config");
    class_test("ConfigTest", fails - before);
    newline();

    before=fails; TimestampTest("Timestamp");
    class_test("TimestampTest", fails - before);
    newline();

    before=fails; WorkspaceTest("Workspace");
    class_test("WorkspaceTest", fails - before);
    newline();

    before=fails; TasklistTest("Tasklist");
    class_test("TasklistTest", fails - before);
    newline();

    before=fails; TaskTest("Task");
    class_test("TaskTest", fails - before);
    newline();

    print_conclusion();

    return 0;
}

// Reload cannot be tested easily, we just gotta trust
void TasklistTest(std::string classname) {
    std::filesystem::path working_folder = 
        "/home/arleok/repos/ntask/tst-working/workspace/";

    print_call(classname, "Tasklist", "tst-work/workspace/", "objptr");
    Tasklist tasklist = Tasklist(working_folder);
    newline();

    // no filter
    print_call(classname, "clear_filters", "", "");
    tasklist.clear_filters();
    print_tasks(tasklist.Tasks(), 4, true, false, false, true, true);
    newline();

    // status filters
    print_call(classname, "set_status_filter", "status_done", "");
    tasklist.set_status_filter(Tasklist::status_done);
    print_tasks(tasklist.Tasks(), 1, true, false, false, true, true);
    newline();

    tasklist.clear_status_filter();

    print_call(classname, "set_status_filter", "status_na", "permanent");
    tasklist.set_status_filter(Tasklist::status_na);
    print_tasks(tasklist.Tasks(), 5, true, true, true, true, true);
    newline();

    // name filters
    print_call(classname, "set_name_filter", "'no match', string_e", "");
    tasklist.set_name_filter("no match");
    tasklist.set_name_type(Tasklist::string_e);
    print_tasks(tasklist.Tasks(), 0, true, false, false, true, false);
    newline();

    print_call(classname, "set_name_filter", "'get 1000000 pts', string_e", "");
    tasklist.set_name_filter("get 1000000 pts");
    tasklist.set_name_type(Tasklist::string_e);
    print_tasks(tasklist.Tasks(), 1, true, false, false, true, false);
    newline();

    print_call(classname, "set_name_filter", "'s', string_ge", "");
    tasklist.set_name_filter("s");
    tasklist.set_name_type(Tasklist::string_ge);
    print_tasks(tasklist.Tasks(), 1, true, false, false, true, false);
    newline();

    print_call(classname, "set_name_filter", "'et', string_fz", "");
    tasklist.set_name_filter("et");
    tasklist.set_name_type(Tasklist::string_fz);
    print_tasks(tasklist.Tasks(), 4, true, false, false, true, false);
    newline();

    tasklist.clear_name_filter();

    // folder filters
    print_call(classname, "set_folder_filter", "'ga', string_ge", "");
    tasklist.set_folder_filter("ga");
    tasklist.set_folder_type(Tasklist::string_ge);
    print_tasks(tasklist.Tasks(), 3, true, false, true, false, false);
    newline();

    tasklist.clear_folder_filter();

    // file filters
    print_call(classname, "set_file_filter", "'lis', string_ge", "");
    tasklist.set_file_filter("lis");
    tasklist.set_file_type(Tasklist::string_ge);
    print_tasks(tasklist.Tasks(), 2, true, false, true, false, false);
    newline();

    tasklist.clear_file_filter();

    // tag filters
    print_call(classname, "set_tag_filter", "'s', string_ge", "");
    tasklist.set_tag_filter("s");
    tasklist.set_tag_type(Tasklist::string_ge);
    print_tasks(tasklist.Tasks(), 2, true, true, false, false, false);
    newline();

    tasklist.clear_tag_filter();

    // date filters
    print_call(classname, "set_date_filter", "'2024.10.15', date_e", "");
    tasklist.set_date_filter(Timestamp("2024.10.15"));
    tasklist.set_date_type(Tasklist::date_e);
    print_tasks(tasklist.Tasks(), 1, true, false, false, true, false);
    newline();

    print_call(classname, "set_date_filter", "'2024.10.15', date_gr", "");
    tasklist.set_date_filter(Timestamp("2024.10.15"));
    tasklist.set_date_type(Tasklist::date_gr);
    print_tasks(tasklist.Tasks(), 2, true, false, false, true, false);
    newline();

    print_call(classname, "set_date_filter", "'2024.10.15', date_ge", "");
    tasklist.set_date_filter(Timestamp("2024.10.15"));
    tasklist.set_date_type(Tasklist::date_ge);
    print_tasks(tasklist.Tasks(), 3, true, false, false, true, false);
    newline();

    print_call(classname, "set_date_filter", "'2024.01.01', date_ls", "");
    tasklist.set_date_filter(Timestamp("2024.01.01"));
    tasklist.set_date_type(Tasklist::date_ls);
    print_tasks(tasklist.Tasks(), 1, true, false, false, true, false);
    newline();

    print_call(classname, "set_date_filter", "'2024.01.01', date_le", "");
    tasklist.clear_date_filter();
    tasklist.set_date_filter(Timestamp("2024.01.01"));
    tasklist.set_date_type(Tasklist::date_le);
    print_tasks(tasklist.Tasks(), 2, true, false, false, true, false);
    newline();

    print_call(classname, "set_date_filter", "'2024.10.15*00:00', date_u2y", "");
    tasklist.clear_date_filter();
    tasklist.set_date_filter(Timestamp("2024.10.15*00:00"));
    tasklist.set_date_type(Tasklist::date_u2y);
    print_tasks(tasklist.Tasks(), 2, true, false, false, true, false);
    newline();

    print_call(classname, "set_date_filter", "'2024.10.15*00:00', date_u2m", "");
    tasklist.clear_date_filter();
    tasklist.set_date_filter(Timestamp("2024.10.15*00:00"));
    tasklist.set_date_type(Tasklist::date_u2m);
    print_tasks(tasklist.Tasks(), 1, true, false, false, true, false);
    newline();

    print_call(classname, "set_date_filter", "'2024.10.15*00:00', date_u2d", "");
    tasklist.clear_date_filter();
    tasklist.set_date_filter(Timestamp("2024.10.15*00:00"));
    tasklist.set_date_type(Tasklist::date_u2d);
    print_tasks(tasklist.Tasks(), 1, true, false, false, true, false);
    newline();

    print_call(classname, "set_date_filter", "'2024.10.15*00:00', date_u2h", "");
    tasklist.clear_date_filter();
    tasklist.set_date_filter(Timestamp("2024.10.15*00:00"));
    tasklist.set_date_type(Tasklist::date_u2h);
    print_tasks(tasklist.Tasks(), 1, true, false, false, true, false);
    newline();

    tasklist.clear_date_filter();

    newline();
}

void WorkspaceTest(std::string classname) {
    std::filesystem::path working_folder = 
        "/home/arleok/repos/ntask/tst-working/workspace/";

    std::vector<Workspace::Block> target_vec; Workspace::Block tmp;

    tmp.content = "\"get 1000000 pts\" {2023.30.10*12}";
    tmp.source_file = working_folder / "gaming/stuffs.norg";
    target_vec.push_back(tmp);

    tmp.content = "\"sutff 100%\" <sutff> {2024}";
    tmp.source_file = working_folder / "gaming/stuffs.norg";
    target_vec.push_back(tmp);

    tmp.content = "\"idk when ill get around to this\" <never>";
    tmp.source_file = working_folder / "gaming/stuffs.norg";
    target_vec.push_back(tmp);

    tmp.content = "\"buy peanut butter cups\" <shopping> {2023.10.17} [2024.10.15]";
    tmp.source_file = working_folder / "shopping/list.norg";
    target_vec.push_back(tmp);

    tmp.content = "\"become #1 *:;. in valorant\" {2020.08.24*16:30}";
    tmp.source_file = working_folder / "shopping/list.norg";
    target_vec.push_back(tmp);

    print_call(classname, "Workspace", "tst-work/workspace/, 'rust task'", "objptr");
    Workspace workspc = Workspace(working_folder, "rust task");
    newline();

    print_call(classname, "ParseWorkspace", "", "");
    workspc.ParseWorkspace();
    print_call(classname, "get_blocks", "", "");
    std::vector<Workspace::Block>* block_vec = workspc.get_blocks();
    print_call("block_vec", "size", "", std::to_string(block_vec->size()));
    make_sure(block_vec->size() == target_vec.size());
    newline();

    for (Workspace::Block block : *block_vec) {
        print_call("block_vec", "element.source_file", "", 
                block.source_file.parent_path().filename().string()+"/"+
                block.source_file.filename().string());
        print_call("block_vec", "element.content", "", block.content);

        bool found = false;
        for (int i = 0; i < target_vec.size(); i++) {
            Workspace::Block target = target_vec[i];

            if (block.content == target.content && 
                    block.source_file == target.source_file) {
                target_vec.erase(target_vec.begin() + i);
                make_sure(true);
                found = true;
                break; // found a match
            }
        }
        if (!found) make_sure(false);

        newline();
    }
}

void TaskTest(std::string classname) {
    std::filesystem::path working_file = 
        "/home/arleok/repos/ntask/tst-working/workspace/shopping/list.norg";

    print_call(classname, "Task", "2023.10.17 tjoes cups", "objptr");
    Task tjoes = Task("\"buy peanut butter cups\" <shopping> {2023.10.17} [2024.10.15]"
            , working_file);

    print_call("tjoes", "get_name", "", tjoes.get_name());
    make_sure(tjoes.get_name() == "buy peanut butter cups");

    print_call("tjoes", "get_tag", "", tjoes.get_tag());
    make_sure(tjoes.get_tag() == "shopping");

    print_call("tjoes", "get_folder", "", tjoes.get_folder());
    make_sure(tjoes.get_folder() == "shopping");

    print_call("tjoes", "get_file", "", tjoes.get_file());
    make_sure(tjoes.get_file() == "list");

    print_call("tjoes", "is_done", "", tjoes.is_done()?"true":"false");
    make_sure(tjoes.is_done());

    print_call("tjoes", "get_date", "", tjoes.get_date().get_str());
    make_sure(tjoes.get_date() == Timestamp("2024.10.15"));


    newline();
    print_call(classname, "Task", "2020.08.24*16:30 symbols & stuff", "objptr");
    Task vlrnt = Task("\"become #1 *:;. in valorant\" {2020.08.24*16:30}", working_file);

    print_call("vlrnt", "get_name", "", vlrnt.get_name());
    make_sure(vlrnt.get_name() == "become #1 *:;. in valorant");

    print_call("vlrnt", "get_tag", "", vlrnt.get_tag());
    make_sure(vlrnt.get_tag() == "");

    print_call("vlrnt", "get_folder", "", vlrnt.get_folder());
    make_sure(vlrnt.get_folder() == "shopping");

    print_call("vlrnt", "get_file", "", vlrnt.get_file());
    make_sure(vlrnt.get_file() == "list");

    print_call("vlrnt", "is_done", "", vlrnt.is_done()?"true":"false");
    make_sure(!vlrnt.is_done());

    print_call("vlrnt", "get_date", "", vlrnt.get_date().get_str());
    make_sure(vlrnt.get_date() == Timestamp("2020.08.24*16:30"));

    print_call("vlrnt", "Complete", "", "");
    vlrnt.Complete();
    std::cout << "MAKE SURE TO CHECK OUTPUT FILE" << std::endl;
    newline();
}

void TimestampTest(std::string classname) {
    std::string datestr_def   = "2023.10.23*23:16";
    std::string datestr_blank = "2023";
    std::string datestr_over  = "2023.00.00*00:00";

    Timestamp now_auto = Timestamp();
    Timestamp now_gen = Timestamp(Timestamp::current_unixtime());
    Timestamp future = Timestamp(true);
    Timestamp zero = Timestamp(false);
    Timestamp fully_def = Timestamp(datestr_def);
    Timestamp part_blank = Timestamp(datestr_blank);
    Timestamp over_def = Timestamp(datestr_over);
    Timestamp empty_str = Timestamp("");

    print_call(classname, "Timestamp", "", now_auto.get_str());
    print_call(classname, "Timestamp", "current_unixtime", now_gen.get_str());
    print_call("now_auto", "get_day", "", std::to_string(now_auto.get_day()));
    print_call("now_auto", "get_month", "", std::to_string(now_auto.get_month()));
    make_sure(now_auto.get_str() == now_gen.get_str(), "strings equal");
    make_sure(now_auto == now_gen, "object equality");
    newline();

    print_call(classname, "Timestamp", "future", future.get_str());
    print_call("future", "is_future", "", future.is_future()? "true":"false");
    make_sure(future.get_year() > 4000, "more than year 4000");
    make_sure(future.is_future(), "isfuture");
    newline();

    print_call(classname, "Timestamp", "future", future.get_str());
    print_call("future", "is_future", "", future.is_future()? "true":"false");
    make_sure(future.get_year() > 4000, "more than year 4000");
    make_sure(future.is_future(), "isfuture");
    newline();

    print_call(classname, "Timestamp", "zero", zero.get_str());
    print_call("zero", "is_zero", "", zero.is_zero()? "true":"false");
    make_sure(zero.get_year() < 2000, "less than year 2000");
    make_sure(zero.is_zero(), "iszero");
    newline();

    print_call(classname, "Timestamp", datestr_def, fully_def.get_str());
    print_call(classname, "Timestamp", datestr_blank, part_blank.get_str());
    print_call(classname, "Timestamp", datestr_over, over_def.get_str());
    make_sure(part_blank == over_def, "part blank equals over defined");
    make_sure(over_def.get_str(false) == part_blank.get_str(false), "non truncated strs equal");
    newline();

    print_call(classname, "get_year", "", std::to_string(fully_def.get_year()));
    make_sure(fully_def.get_year() == 2023);
    print_call(classname, "get_month", "", std::to_string(fully_def.get_month()));
    make_sure(fully_def.get_month() == 10);
    print_call(classname, "get_date", "", std::to_string(fully_def.get_date()));
    make_sure(fully_def.get_date() == 23);
    print_call(classname, "get_day", "", std::to_string(fully_def.get_day()));
    make_sure(fully_def.get_day() == 1);
    print_call(classname, "get_hour", "", std::to_string(fully_def.get_hour()));
    make_sure(fully_def.get_hour() == 23);
    print_call(classname, "get_min", "", std::to_string(fully_def.get_min()));
    make_sure(fully_def.get_min() == 16);
    newline();
}

void ConfigTest(std::string classname) {
    Config config;
    
    print_call(classname, "Read", "/home/arleok/repos/ntask/tst-working/config");
    try {
        config.Read((std::filesystem::path) "/home/arleok/repos/ntask/tst-working/config");
    } catch (...) {
        make_sure(false);
        newline();
        return;
    }
    make_sure(true);
    newline();

    std::string str = config.get_str("username");
    print_call(classname, "get_str", "username", str);
    make_sure(str == "arleok");
    newline();

    int integer = config.get_int("color1");
    print_call(classname, "get_int", "color1", std::to_string(integer));
    make_sure(integer == 5);
    newline();

    std::string path = config.get_path("workspace").string();
    print_call(classname, "get_path", "workspace", path);
    make_sure(path == "/home/arleok/wiki/");
    newline();
}

void print_conclusion() {
    std::cout << "-----------------------------------------" << std::endl;
    std::cout << "Total test fail count: " << fails << " / " << testcount << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
}

void class_test(std::string testname, int failcount) {
    std::cout << "#########################################" << std::endl;
    std::cout << testname << " fail count: " << std::to_string(failcount) << std::endl;
    std::cout << "#########################################" << std::endl;
}

void make_sure(bool condition) {
    make_sure(condition, "");
}

void make_sure(bool condition, std::string info) { testcount++;
    if (condition) {
        std::cout << "success (" << info << ")" << std::endl;
    } else {
        fails++;
        std::cout << "FAIL (" << info << ")" << std::endl;
    }
}

void print_tasks(std::vector<Task>* list, int len, bool name, bool tag, bool path, bool date, bool status) {
    for (Task t : *list) {
        std::string out;
        if (name) out += "\"" + t.get_name() + "\" ";
        if (tag) out += "<" + t.get_tag() + "> ";
        if (path) out += ":" + t.get_folder() + "/" + t.get_file() + ": ";
        if (date) out += "{" + t.get_date().get_str() + "} ";
        if (status) out += (t.is_done()?"done ":"due ");
        std::cout << out << std::endl;
    }
    make_sure(list->size() == len);
}

void print_call(
        std::string class_str, 
        std::string func_str,
        std::string arg_str,
        std::string result) {

    if (result.empty()) {
        std::cout << class_str << "." << func_str << "(" << arg_str << ")" << std::endl;
    } else {
        std::cout << class_str << "." << func_str << "(" << arg_str << ") = " << result << std::endl;
    }
}

void print_call( std::string class_str, std::string func_str, std::string arg_str) {
    std::cout << class_str << "." << func_str << "(" << arg_str << ")" << std::endl;
}

void newline() { newline(1); }
void newline(int j) { for (int i = 0; i < j; i++) std::cout << std::endl; }
