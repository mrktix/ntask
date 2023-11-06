#include "Test.h"

int main(int argc, char** argv) {
    int total_fail = 0;

    newline();

    class_test("ConfigTest", ConfigTest("Config"), total_fail);
    newline();

    class_test("TimestampTest", TimestampTest("Timestamp"), total_fail);
    newline();

    class_test("TaskTest", TaskTest("Task"), total_fail);
    newline();

    class_test("WorkspaceTest", WorkspaceTest("Workspace"), total_fail);
    newline();

    print_conclusion(total_fail);

    return 0;
}

int WorkspaceTest(std::string classname) {
    int fails = 0;

    std::filesystem::path working_folder = 
        "/home/arleok/repos/ntask/tst-working/workspace/";

    print_call(classname, "Workspace", "tst-work/workspace/, 'rust task'", "objptr");
    Workspace workspc = Workspace(working_folder, "rust task");
    newline();

    print_call(classname, "ParseWorkspace", "", "");
    workspc.ParseWorkspace();
    print_call(classname, "get_blocks", "", "");
    std::vector<Workspace::Block>* block_vec = workspc.get_blocks();
    print_call("block_vec", "size", "", std::to_string(block_vec->size()));
    make_sure(block_vec->size() == 4, fails);
    newline();

    std::vector<Workspace::Block> target; Workspace::Block tmp;

    tmp.content = "\"get 1000000 pts\" {2023.30.10*12}";
    tmp.source_file = working_folder / "gaming/stuffs.norg";
    target.push_back(tmp);

    tmp.content = "\"sutff 100%\" <sutff> {2024}";
    tmp.source_file = working_folder / "gaming/stuffs.norg";
    target.push_back(tmp);

    tmp.content = "\"buy peanut butter cups\" <shopping> {2023.10.17} [2024.10.15]";
    tmp.source_file = working_folder / "shopping/list.norg";
    target.push_back(tmp);

    tmp.content = "\"become #1 *:;. in valorant\" {2020.08.24*16:30}";
    tmp.source_file = working_folder / "shopping/list.norg";
    target.push_back(tmp);

    for (Workspace::Block block : *block_vec) {
    }
    
    return fails;
}

int TaskTest(std::string classname) {
    int fails = 0;

    std::filesystem::path working_file = 
        "/home/arleok/repos/ntask/tst-working/workspace/shopping/list.norg";

    print_call(classname, "Task", "2023.10.17 tjoes cups", "objptr");
    Task tjoes = Task("\"buy peanut butter cups\" <shopping> {2023.10.17} [2024.10.15]"
            , working_file);

    print_call("tjoes", "get_name", "", tjoes.get_name());
    make_sure(tjoes.get_name() == "buy peanut butter cups", fails);

    print_call("tjoes", "get_tag", "", tjoes.get_tag());
    make_sure(tjoes.get_tag() == "shopping", fails);

    print_call("tjoes", "get_folder", "", tjoes.get_folder());
    make_sure(tjoes.get_folder() == "shopping", fails);

    print_call("tjoes", "get_file", "", tjoes.get_file());
    make_sure(tjoes.get_file() == "list", fails);

    print_call("tjoes", "is_done", "", tjoes.is_done()?"true":"false");
    make_sure(tjoes.is_done(), fails);

    print_call("tjoes", "get_date", "", tjoes.get_date().get_str());
    make_sure(tjoes.get_date() == Timestamp("2024.10.15"), fails);


    newline();
    print_call(classname, "Task", "2020.08.24*16:30 symbols & stuff", "objptr");
    Task vlrnt = Task("\"become #1 *:;. in valorant\" {2020.08.24*16:30}", working_file);

    print_call("vlrnt", "get_name", "", vlrnt.get_name());
    make_sure(vlrnt.get_name() == "become #1 *:;. in valorant", fails);

    print_call("vlrnt", "get_tag", "", vlrnt.get_tag());
    make_sure(vlrnt.get_tag() == "", fails);

    print_call("vlrnt", "get_folder", "", vlrnt.get_folder());
    make_sure(vlrnt.get_folder() == "shopping", fails);

    print_call("vlrnt", "get_file", "", vlrnt.get_file());
    make_sure(vlrnt.get_file() == "list", fails);

    print_call("vlrnt", "is_done", "", vlrnt.is_done()?"true":"false");
    make_sure(!vlrnt.is_done(), fails);

    print_call("vlrnt", "get_date", "", vlrnt.get_date().get_str());
    make_sure(vlrnt.get_date() == Timestamp("2020.08.24*16:30"), fails);

    print_call("vlrnt", "Complete", "", "");
    vlrnt.Complete();
    std::cout << "MAKE SURE TO CHECK OUTPUT FILE" << std::endl;
    newline();

    return fails;
}

int TimestampTest(std::string classname) {
    int fails = 0;

    std::string datestr_def   = "2023.10.23*23:16";
    std::string datestr_blank = "2023.10.23";
    std::string datestr_over  = "2023.10.23*00:00";

    Timestamp now_auto = Timestamp();
    Timestamp now_gen = Timestamp(Timestamp::current_unixtime());
    Timestamp future = Timestamp(true);
    Timestamp zero = Timestamp(false);
    Timestamp fully_def = Timestamp(datestr_def);
    Timestamp part_blank = Timestamp(datestr_blank);
    Timestamp over_def = Timestamp(datestr_over);

    print_call(classname, "Timestamp", "", now_auto.get_str());
    print_call(classname, "Timestamp", "current_unixtime", now_gen.get_str());
    print_call("now_auto", "get_day", "", std::to_string(now_auto.get_day()));
    print_call("now_auto", "get_month", "", std::to_string(now_auto.get_month()));
    make_sure(now_auto.get_str() == now_gen.get_str(), fails, "strings equal");
    make_sure(now_auto == now_gen, fails, "object equality");
    newline();

    print_call(classname, "Timestamp", "future", future.get_str());
    print_call("future", "is_future", "", future.is_future()? "true":"false");
    make_sure(future.get_year() > 4000, fails, "more than year 4000");
    make_sure(future.is_future(), fails, "isfuture");
    newline();

    print_call(classname, "Timestamp", "zero", zero.get_str());
    print_call("zero", "is_zero", "", zero.is_zero()? "true":"false");
    make_sure(zero.get_year() < 2000, fails, "less than year 2000");
    make_sure(zero.is_zero(), fails, "iszero");
    newline();

    print_call(classname, "Timestamp", datestr_def, fully_def.get_str());
    print_call(classname, "Timestamp", datestr_blank, part_blank.get_str());
    print_call(classname, "Timestamp", datestr_over, over_def.get_str());
    make_sure(part_blank == over_def, fails, "part blank equals over defined");
    make_sure(over_def.get_str(false) == part_blank.get_str(false), fails, "non truncated strs equal");
    newline();

    print_call(classname, "get_year", "", std::to_string(fully_def.get_year()));
    make_sure(fully_def.get_year() == 2023, fails);
    print_call(classname, "get_month", "", std::to_string(fully_def.get_month()));
    make_sure(fully_def.get_month() == 10, fails);
    print_call(classname, "get_date", "", std::to_string(fully_def.get_date()));
    make_sure(fully_def.get_date() == 23, fails);
    print_call(classname, "get_day", "", std::to_string(fully_def.get_day()));
    make_sure(fully_def.get_day() == 1, fails);
    print_call(classname, "get_hour", "", std::to_string(fully_def.get_hour()));
    make_sure(fully_def.get_hour() == 23, fails);
    print_call(classname, "get_min", "", std::to_string(fully_def.get_min()));
    make_sure(fully_def.get_min() == 16, fails);
    newline();

    return fails;
}

int ConfigTest(std::string classname) {
    int fails = 0;
    Config config;
    
    print_call(classname, "Read", "/home/arleok/repos/ntask/tst-working/config");
    try {
        config.Read((std::filesystem::path) "/home/arleok/repos/ntask/tst-working/config");
    } catch (...) {
        make_sure(false, fails);
        newline();
        return fails;
    }
    make_sure(true, fails);
    newline();

    std::string str = config.get_str("username");
    print_call(classname, "get_str", "username", str);
    make_sure(str == "arleok", fails);
    newline();

    int integer = config.get_int("color1");
    print_call(classname, "get_int", "color1", std::to_string(integer));
    make_sure(integer == 5, fails);
    newline();

    std::string path = config.get_path("workspace").string();
    print_call(classname, "get_path", "workspace", path);
    make_sure(path == "/home/arleok/wiki/", fails);
    newline();

    return fails;
}

void print_conclusion(int fail) {
    std::cout << "-----------------------------------------" << std::endl;
    std::cout << "Total test fail count: " << std::to_string(fail) << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
}

void class_test(std::string testname, int fail, int& ref_total_fail) {
    std::cout << "#########################################" << std::endl;
    std::cout << testname << " fail count: " << std::to_string(fail) << std::endl;
    std::cout << "#########################################" << std::endl;
    ref_total_fail += fail;
}

void make_sure(bool condition, int& ref_fail) {
    make_sure(condition, ref_fail, "");
}

void make_sure(bool condition, int& ref_fail, std::string info) {
    if (condition) {
        std::cout << "success (" << info << ")" << std::endl;
    } else {
        ref_fail++;
        std::cout << "FAIL (" << info << ")" << std::endl;
    }
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
