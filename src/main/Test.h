#include "../back/Config.h"
#include "../back/Timestamp.h"
#include "../back/Tasklist.h"
#include "../back/Task.h"
#include "../back/Workspace.h"

#include <iostream>
#include <string>

int fails;
int testcount;

void ConfigTest(std::string classname);
void TaskTest(std::string classname);
void TimestampTest(std::string classname);
void WorkspaceTest(std::string classname);
void TasklistTest(std::string classname);

void class_test(std::string testname, int failcount);
void make_sure(bool condition);
void make_sure(bool condition, std::string info);
void print_conclusion();
void print_tasks(std::vector<Task>* list, int len, bool name, bool tag, bool path, bool date, bool status); // name tag path date status
void print_call(std::string class_str, std::string func_str, std::string arg_str);
void print_call( std::string class_str, std::string func_str, std::string arg_str,
        std::string result);
void newline();
void newline(int j);
