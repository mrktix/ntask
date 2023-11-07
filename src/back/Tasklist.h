#pragma once

#include "Task.h"
#include "Timestamp.h"
#include "Workspace.h"

#include <filesystem>
#include <vector>

class Tasklist {
    public:
        Tasklist(std::filesystem::path workspace);

        // deterministic list of tasks, filtered by the filters specified
        std::vector<Task>* Tasks();

        // reload the task list inplace (DOES NOT REFILTER)
        void Reload();

        enum DateType {
            date_na, // filter should not be applied
            date_gr, // greater dates match
            date_ge, // greater or equal dates match
            date_ls, // less dates match
            date_le, // less or equal dates match
            date_u2y, // up to year, date matches
            date_u2m, // up to month, date matches
            date_u2d, // up to day, date matches
            date_u2h, // up to hour, date matches
            date_u2n,// up to min, date matches
            date_e, //  dates match completely
        };

        enum StringType {
            string_na, // filter not applied, anything matches
            string_e, // exact match
            string_ge, // matches anything containing pattern
            string_fz, // fuzzy match (*p*a*t*t*e*r*n*)
        };

        enum DueStatus {
            status_na, // filter not applied, anything matches (done shows up after due)
            status_done,
            status_due,
        };

        void set_name_filter(std::string match);
        void set_folder_filter(std::string match);
        void set_file_filter(std::string match);
        void set_tag_filter(std::string match);
        void set_status_filter(DueStatus match);

        void set_name_type(StringType typ);
        void set_folder_type(StringType typ);
        void set_file_type(StringType typ);
        void set_tag_type(StringType typ);

        void set_date_filter(Timestamp match);
        void set_date_type(DateType typ);

    private:
        Workspace norg_workspace;
        std::vector<Task> tasks_done; // sorted by date, greatest first
        std::vector<Task> tasks_due; // sorted by date, least first
        std::vector<Task> tasks_filtered;
        bool filtered_tasks_up_to_date;

        DueStatus due_typ;
        std::string name_pat, folder_pat, file_pat, tag_pat;
        StringType name_typ, folder_typ, file_typ, tag_typ;
        Timestamp date_cut;
        DateType date_typ;

        bool match_filter(Task task) const;
        bool match_name(Task task) const;
        bool match_folder(Task task) const;
        bool match_file(Task task) const;
        bool match_tag(Task task) const;
        bool match_date(Task task) const;
        bool match_due(Task task) const;

        // return true if pattern fuzzy matches into text
        static bool string_fuzzy_match(std::string pattern, std::string text);

        void Filter(); // apply given filters and regenerate vector
        void InsertUnfiltered(Task new_task); 
        // insert a task into the corresponding vector, sorting only by date
};
