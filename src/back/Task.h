#pragma once

#include "Timestamp.h"

#include <boost/algorithm/string/trim.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

class Task {
    public:
        Task(std::string definition, std::filesystem::path source_file);

        std::string get_name() const;
        std::string get_tag() const;
        std::string get_folder() const;
        std::string get_file() const;
        Timestamp get_date() const;
        bool is_done() const;

        void Complete();

    private:
        void Write() const;
        int LineNumber() const;

        std::filesystem::path source_file;

        std::string name;
        std::string tag;
        Timestamp date_due;
        Timestamp date_done;
        bool done;
};
