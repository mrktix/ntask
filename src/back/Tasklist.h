#pragma once

#include "Task.h"

#include <filesystem>
#include <vector>

class Tasklist {
    public:
        Tasklist(std::filesystem::path workspace);

        // deterministic list of tasks, filtered by the filters specified
        // filtering specified in docs
        std::vector<Task> Tasks(std::string filter);
        std::vector<Task> Tasks(const char filter[]);

    private:
}
