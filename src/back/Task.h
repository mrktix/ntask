#pragma once

#include "Timestamp.h"
#include "Workspace.h"

#include <boost/algorithm/string/trim.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <vector>

class Task {
    public:
        Task(std::string definition, std::filesystem::path source_file);
        Task(Workspace::Block definition);

        std::string get_name() const;
        std::string get_tag() const;
        std::string get_folder() const;
        std::string get_file() const;
        Timestamp get_date() const;
        bool is_done() const;

        void Complete();

        bool operator<(const Task& rhs) const;
        bool operator<=(const Task& rhs) const;
        bool operator==(const Task& rhs) const;
        bool operator>=(const Task& rhs) const;
        bool operator>(const Task& rhs) const;
        bool operator!=(const Task& rhs) const;


    private:
        void Construct();
        void Write(bool replace_which[4]) const;
        int LineNumber() const;

        static std::string field_start_char[4];
        static std::string field_end_char[4];

        std::filesystem::path source_file;

        std::string name;
        std::string tag;
        Timestamp date_due;
        Timestamp date_done;
        bool done;
};
