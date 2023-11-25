#include "Tasklist.h"

Tasklist::Tasklist(std::filesystem::path workspace_path) {
    filtered_tasks_up_to_date = true;
    name_typ = folder_typ = file_typ = tag_typ = string_na;
    date_typ = date_na;
    due_typ = status_due;
    norg_workspace = new Workspace(workspace_path, "rust task");
    Reload();
    // vec<Task> tasks, tasks_filtered
}

Tasklist::~Tasklist() {
    delete norg_workspace;
}

std::vector<Task>* Tasklist::Tasks() {
    // if we have reloaded or changed filters since last filter, refilter
    if (!filtered_tasks_up_to_date) Filter();
    return &tasks_filtered;
}

// reloads the unfiltered lists, tasks_done & _due
void Tasklist::Reload() {
    norg_workspace->ParseWorkspace();
    std::vector<Workspace::Block>* block_vec = norg_workspace->get_blocks();
    tasks_due.clear(); tasks_done.clear(); filtered_tasks_up_to_date = false;
    
    // parse the workspace for task blocks and parse them into tasks,
    // sorting by date into the correct vector
    for (int i = 0; i < block_vec->size(); i++) {
        InsertUnfiltered(Task(block_vec->at(i)));
    }
}

// priv
void Tasklist::InsertUnfiltered(Task new_task) {
    bool inserted = false;
    if (new_task.is_done()) {
        for (int i = 0; i < tasks_done.size(); i++) {
            // only insert once this task is greater or same as the next element
            // so it will always add to start of equal blocks
            if (new_task >= tasks_done[i]) {
                tasks_done.insert(tasks_done.begin() + i, new_task);
                inserted = true;
                break;
            }
        }
        if (!inserted) tasks_done.push_back(new_task);
    } else { // task is due, not done yet
        for (int i = 0; i < tasks_due.size(); i++) {
            // only insert once this task is less than next task
            // so it will always add to end of equal blocks
            if (new_task < tasks_due[i]) {
                tasks_due.insert(tasks_due.begin() + i, new_task);
                inserted = true;
                break;
            }
        }
        if (!inserted) tasks_due.push_back(new_task);
    }
}

// should not be called other than line 1 of Tasks()
void Tasklist::Filter() { // priv
    tasks_filtered.clear(); filtered_tasks_up_to_date = true;

    // depending on filters, source from done/due tasks,
    // and only add tasks that match rest of filter
    if (due_typ == status_done || due_typ == status_na) {
        for (Task task : tasks_done) { if (match_filter(task)) {
            tasks_filtered.push_back(task);
        }}
    }
    if (due_typ == status_due || due_typ == status_na) {
        for (Task task : tasks_due) { if (match_filter(task)) {
            tasks_filtered.push_back(task);
        }}
    }
}

bool Tasklist::string_match(std::string pattern, std::string text, Tasklist::StringType strtype) const {
    switch (strtype) {
        case Tasklist::string_na: // anything matches
            return true; break;
        case Tasklist::string_e: // need exact match
            return text == pattern; break;
        case Tasklist::string_ge: // text must contain pattern at the start
            return text.find(pattern) == 0; break;
        case Tasklist::string_fz: // fuzzy matching
            return string_fuzzy_match(pattern, text); break;
    }
}

bool Tasklist::string_fuzzy_match(std::string pattern, std::string text) const {
    for (char c : text) {
        if (c == pattern[0]) pattern.erase(0,1);

        if (pattern.empty()) return true;
    }

    return false;
}

bool Tasklist::match_filter(Task task) const {
    return match_name(task)
        && match_folder(task)
        && match_file(task)
        && match_tag(task)
        && match_date(task);
}

bool Tasklist::match_name(Task task) const {
    return string_match(name_pat, task.get_name(), name_typ);
}

bool Tasklist::match_folder(Task task) const {
    return string_match(folder_pat, task.get_folder(), folder_typ);
}

bool Tasklist::match_file(Task task) const {
    return string_match(file_pat, task.get_file(), file_typ);
}

bool Tasklist::match_tag(Task task) const {
    return string_match(tag_pat, task.get_tag(), tag_typ);
}

bool Tasklist::match_date(Task task) const {
    Timestamp date_task = task.get_date();

    switch(date_typ) {
        case date_na: 
            return true; break;
        case date_e:
            return date_task == date_cut; break;

        case date_gr:
            return date_task > date_cut; break;
        case date_ge:
            return date_task >= date_cut; break;
        case date_ls:
            return date_task < date_cut; break;
        case date_le:
            return date_task <= date_cut; break;

        case date_u2y:
            return date_task.get_year() == date_cut.get_year();
            break;
        case date_u2m:
            return date_task.get_year() == date_cut.get_year()
                && date_task.get_month() == date_cut.get_month();
            break;
        case date_u2d:
            return date_task.get_year() == date_cut.get_year()
                && date_task.get_month() == date_cut.get_month()
                && date_task.get_date() == date_cut.get_date();
            break;
        case date_u2h:
            return date_task.get_year() == date_cut.get_year()
                && date_task.get_month() == date_cut.get_month()
                && date_task.get_date() == date_cut.get_date()
                && date_task.get_hour() == date_cut.get_hour();
            break;
    }

    return false;
}

void Tasklist::set_name_filter  (std::string match)
{ filtered_tasks_up_to_date = false; name_pat = match; }
void Tasklist::set_folder_filter(std::string match)
{ filtered_tasks_up_to_date = false; folder_pat = match; }
void Tasklist::set_file_filter  (std::string match)
{ filtered_tasks_up_to_date = false; file_pat = match; }
void Tasklist::set_tag_filter   (std::string match)
{ filtered_tasks_up_to_date = false; tag_pat = match; }
void Tasklist::set_date_filter  (Timestamp match)
{ filtered_tasks_up_to_date = false; date_cut = match; }

void Tasklist::set_status_filter(DueStatus match)
{ filtered_tasks_up_to_date = false; due_typ = match; }

void Tasklist::set_name_type  (StringType typ)
{ filtered_tasks_up_to_date = false; name_typ = typ; }
void Tasklist::set_folder_type(StringType typ)
{ filtered_tasks_up_to_date = false; folder_typ = typ; }
void Tasklist::set_file_type  (StringType typ)
{ filtered_tasks_up_to_date = false; file_typ = typ; }
void Tasklist::set_tag_type   (StringType typ)
{ filtered_tasks_up_to_date = false; tag_typ = typ; }
void Tasklist::set_date_type  (DateType typ)
{ filtered_tasks_up_to_date = false; date_typ = typ; }

void Tasklist::clear_filters() {
    clear_name_filter();
    clear_folder_filter();
    clear_file_filter();
    clear_tag_filter();
    clear_date_filter();
    clear_status_filter();
}
void Tasklist::clear_name_filter()
{ filtered_tasks_up_to_date = false; name_typ = string_na; }
void Tasklist::clear_folder_filter()
{ filtered_tasks_up_to_date = false; folder_typ = string_na; }
void Tasklist::clear_file_filter()
{ filtered_tasks_up_to_date = false; file_typ = string_na; }
void Tasklist::clear_tag_filter()
{ filtered_tasks_up_to_date = false; tag_typ = string_na; }
void Tasklist::clear_date_filter()
{ filtered_tasks_up_to_date = false; date_typ = date_na; }
void Tasklist::clear_status_filter()
{ filtered_tasks_up_to_date = false; due_typ = status_due; }
