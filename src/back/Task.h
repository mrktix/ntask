#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

class Task {
    public:
        Task();

        void Read(std::string definition, std::filesystem::path source_file);

        std::string get_name() const;
        std::string get_tag() const;
        std::string get_folder() const;
        std::string get_file() const;
        long get_date() const;
        bool is_done() const;

        void set_done_date(long new_done_date);

    private:
        void Write() const;

        std::filesystem::path source_file;

        std::string name;
        std::string tag;
        long date;
        bool done;

        static long future_date;
};
