#include "../back/Config.h"
#include "../back/Timestamp.h"
#include "../back/Task.h"

#include <iostream>
#include <string>

int ConfigTest(std::string classname);
int TaskTest(std::string classname);
int TimestampTest(std::string classname);

void class_test(std::string testname, int fail, int& ref_total_fail);
void make_sure(bool condition, int& ref_fail);
void make_sure(bool condition, int& ref_fail, std::string info);
void print_conclusion(int fail);
void print_call(std::string class_str, std::string func_str, std::string arg_str);
void print_call(
        std::string class_str, 
        std::string func_str,
        std::string arg_str,
        std::string result);
void newline();
void newline(int j);
