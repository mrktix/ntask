#include "Tasklist.h"

Tasklist::Tasklist(std::filesystem::path workspace) {
    tasks_up_to_date = true;
    name_typ = folder_typ = file_typ = tag_typ = string_na;
    date_typ = date_na;
    due_typ = status_due;
}

std::vector<Task>* Tasklist::Tasks() {
    if (!tasks_up_to_date) Filter();

    return &tasks_filtered;
}

void Tasklist::Reload() {

}

void Tasklist::Filter() { // priv

}

void Tasklist::set_name_filter(std::string match) { name_pat = match; }
void Tasklist::set_folder_filter(std::string match) { folder_pat = match; }
void Tasklist::set_file_filter(std::string match) { file_pat = match; }
void Tasklist::set_tag_filter(std::string match) { tag_pat = match; }
void Tasklist::set_date_filter(Timestamp match) { date_cut = match; }

void Tasklist::set_status_filter(DueStatus match) { due_typ = match; }

void Tasklist::set_name_type(StringType typ) { name_typ = typ; }
void Tasklist::set_folder_type(StringType typ) { folder_typ = typ; }
void Tasklist::set_file_type(StringType typ) { file_typ = typ; }
void Tasklist::set_tag_type(StringType typ) { tag_typ = typ; }
void Tasklist::set_date_type(DateType typ) { date_typ = typ; }
