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

std::vector<Task>* Tasklist::Tasks() {
    // if we have reloaded or changed filters since last filter, refilter
    if (!filtered_tasks_up_to_date) Filter();
    return &tasks_filtered;
}

void Tasklist::Reload() {
    norg_workspace.ParseWorkspace();
    std::vector<Workspace::Block>* block_vec = norg_workspace.get_blocks();
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
                inserted = false;
            }
        }
        if (!inserted) tasks_done.push_back(new_task);
    } else {
        for (int i = 0; i < tasks_done.size(); i++) {
            // only insert once this task is less than next task
            // so it will always add to end of equal blocks
            if (new_task < tasks_done[i]) {
                tasks_done.insert(tasks_done.begin() + i, new_task);
                inserted = false;
            }
        }
        if (!inserted) tasks_done.push_back(new_task);
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

bool Tasklist::match_filter(Task task) const {
    return match_name(task)
        && match_folder(task)
        && match_file(task)
        && match_tag(task)
        && match_date(task);
}

bool Tasklist::match_name(Task task) const {
    switch (name_typ) {
        case string_na: // anything matches
            return true; break;
        case string_e: // need exact match
            return task.get_name() == name_pat; break;
        case string_ge: // name must contain pattern
            return task.get_name().find(name_pat) != -1; break;
        case string_fz: // fuzzy matching
            break;
    }

}

bool Tasklist::match_folder(Task task) const {

}

bool Tasklist::match_file(Task task) const {

}

bool Tasklist::match_tag(Task task) const {

}

bool Tasklist::match_date(Task task) const {

}

bool Tasklist::string_fuzzy_match(std::string pattern, std::string text) {
    for (char c : text) {
        if (c == pattern[0])

        if (pattern.empty()) return true;
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
