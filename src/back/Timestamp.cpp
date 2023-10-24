#include "Timestamp.h"
#include <string>
#include <tuple>

long Timestamp::futuretime = 0;
// struct tm tm

void Timestamp::InitFuturetime() {
    if (futuretime == 0) futuretime = current_unixtime()+65000000000; // ~2000 years in future
}

Timestamp::Timestamp() {
    long current_utime = current_unixtime();
    tm = *std::localtime(&current_utime);
    tm.tm_sec = 0;
}

Timestamp::Timestamp(bool future) {
    InitFuturetime();
    if (future) {
        tm = *std::localtime(&futuretime);
    } else {
        tm = *std::localtime(0);
    }
    tm.tm_sec = 0;
}

Timestamp::Timestamp(long unixtime) {
    InitFuturetime();
    tm = *std::localtime(&unixtime);
    tm.tm_sec = 0;
}

// datestr : "yyyy.mm.dd*hh:nn"
Timestamp::Timestamp(std::string datestr) {
    InitFuturetime();

    enum stage {
        stage_year, stage_month, stage_day, stage_hour, stage_min,
    } parsing_stage = stage_year;

    tm.tm_isdst = -1;
    tm.tm_year = tm.tm_mon = tm.tm_mday = tm.tm_hour = tm.tm_min = tm.tm_sec = 0;

    for (int i = 0; i < datestr.length(); i++) {
        char ch = datestr[i];

        if (ch == '.') {
            switch (parsing_stage) {
                case stage_year: parsing_stage = stage_month; break;
                case stage_month: parsing_stage = stage_day; break;
            }
        } else if (ch == '*') {
            parsing_stage = stage_hour;
        } else if (ch == ':') {
            parsing_stage = stage_min;
        } else {
            switch (parsing_stage) {
                case stage_year:  tm.tm_year *= 10; tm.tm_year += ch - '0'; break;
                case stage_month: tm.tm_mon  *= 10; tm.tm_mon  += ch - '0'; break;
                case stage_day:   tm.tm_mday *= 10; tm.tm_mday += ch - '0'; break;
                case stage_hour:  tm.tm_hour *= 10; tm.tm_hour += ch - '0'; break;
                case stage_min:   tm.tm_min  *= 10; tm.tm_min  += ch - '0'; break;
            }
        }
    }
}

long Timestamp::current_unixtime() const {
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

std::string Timestamp::get_str() const {
    std::string ret;
    ret += std::to_string(get_year());
    ret += ".";
    ret += std::to_string(get_month());
    ret += ".";
    ret += std::to_string(get_date());
    ret += "*";
    ret += std::to_string(get_hour());
    ret += ":";
    ret += std::to_string(get_min());

    return ret;
}

long Timestamp::get_unixtime() const {
    struct tm temp = tm;
    temp.tm_year -= 1900;
    temp.tm_mon -= 1;

    return mktime(&temp);
}

bool Timestamp::is_future() const { return get_unixtime() == futuretime; }
int Timestamp::get_year() const { return tm.tm_year; }
int Timestamp::get_month() const { return tm.tm_mon; }
int Timestamp::get_date() const { return tm.tm_mday; }
int Timestamp::get_day() const { /*week day*/ return (tm.tm_wday == 0)? 7 : tm.tm_wday; }
int Timestamp::get_hour() const { return tm.tm_hour; }
int Timestamp::get_min() const { return tm.tm_min; }
