#include "Timestamp.h"

long Timestamp::futuretime = 0;

void Timestamp::TmFromUnix(long unixtime) {
    tm = *std::localtime(&unixtime);

    tm.tm_isdst = -1;
    tm.tm_year += 1900;
    tm.tm_mon += 1;
    tm.tm_sec = 0;
}

void Timestamp::TmFromStr(std::string datestr) {
    enum stage {
        stage_year, stage_month, stage_day, stage_hour, stage_min,
    } parsing_stage = stage_year;

    tm.tm_isdst = -1;
    tm.tm_year = tm.tm_mon = tm.tm_mday = tm.tm_hour = tm.tm_min = tm.tm_sec = 0;

    int year, mon, date, hour, min, sec;

    for (int i = 0; i < datestr.length(); i++) {
        char ch = datestr[i];

        if (ch == '.') {
            switch (parsing_stage) {
                case stage_year: parsing_stage = stage_month; break;
                case stage_month: parsing_stage = stage_day; break;
                case stage_day: break;
                case stage_hour: break;
                case stage_min: break;
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

    tm.tm_year -= 1900;
    tm.tm_mon -= 1;

    long unixtime = std::mktime(&tm);

    TmFromUnix(unixtime);
}

void Timestamp::InitFuturetime() {
    if (futuretime == 0) {
        futuretime = current_unixtime()+94672800000; // 3000 years in future
        futuretime -= futuretime%60; // truncate off seconds
    }
}

Timestamp::Timestamp() { InitFuturetime();
    long current_utime = current_unixtime();
    TmFromUnix(current_utime);
}

Timestamp::Timestamp(bool future) { InitFuturetime();
    if (future) TmFromUnix(futuretime);
    else TmFromUnix(0);
}

Timestamp::Timestamp(long unixtime) { InitFuturetime();
    TmFromUnix(unixtime);
}

Timestamp::Timestamp(const char datestr[]) { InitFuturetime();
    TmFromStr((std::string) datestr);
}

Timestamp::Timestamp(std::string datestr) { InitFuturetime();
    TmFromStr(datestr);
}

long Timestamp::current_unixtime() {
    return std::chrono::duration_cast<std::chrono::seconds>
        (std::chrono::system_clock::now().time_since_epoch())
        .count();
}

std::string Timestamp::get_str() const {
    return get_str(true);
}

std::string Timestamp::get_str(bool truncate) const {
    std::string year = std::to_string(get_year());
    std::string mon = std::to_string(get_month());
    std::string date = std::to_string(get_date());
    std::string hour = std::to_string(get_hour());
    std::string min = std::to_string(get_min());
    std::string ret = "";

    if (year.size() == 1) year = "000" + year;
    if (year.size() == 2) year = "00" + year;
    if (year.size() == 3) year = "0" + year;

    if (mon.size() == 1)  mon  = "0" + mon;
    if (date.size() == 1) date = "0" + date;
    if (hour.size() == 1) hour = "0" + hour;
    if (min.size() == 1)  min  = "0" + min;

    if (!truncate||min  != "00") { if (min  != "00") truncate=false; ret = ":"+min +ret; }
    if (!truncate||hour != "00") { if (hour != "00") truncate=false; ret = "*"+hour+ret; }
    if (!truncate||date != "00") { if (date != "00") truncate=false; ret = "."+date+ret; }
    if (!truncate||mon  != "00") { if (mon  != "00") truncate=false; ret = "."+mon +ret; }
    if (!truncate||year!="0000") { if (year!="0000") truncate=false; ret =     year+ret; }

    return ret;
}

long Timestamp::get_unixtime() const {
    struct tm temp = tm;
    temp.tm_year -= 1900;
    temp.tm_mon -= 1;

    return mktime(&temp);
}

bool Timestamp::is_future() const { return get_unixtime() == futuretime; }
bool Timestamp::is_zero() const { return get_unixtime() == 0; }
int Timestamp::get_year() const { return tm.tm_year; }
int Timestamp::get_month() const { return tm.tm_mon; }
int Timestamp::get_date() const { return tm.tm_mday; }
int Timestamp::get_day() const { return (tm.tm_wday == 0)? 7 : tm.tm_wday; } // weekday
int Timestamp::get_hour() const { return tm.tm_hour; }
int Timestamp::get_min() const { return tm.tm_min; }

bool Timestamp::operator==(const Timestamp& rhs) const {
    return rhs.get_unixtime() == get_unixtime();
}
bool Timestamp::operator<(const Timestamp& rhs) const {
    return rhs.get_unixtime() < get_unixtime();
}

bool Timestamp::operator!=(const Timestamp& rhs) const { return !(*this == rhs); }
bool Timestamp::operator>=(const Timestamp& rhs) const { return !(*this < rhs); }
bool Timestamp::operator>(const Timestamp& rhs) const { return !(*this < rhs || *this == rhs); }
bool Timestamp::operator<=(const Timestamp& rhs) const { return *this < rhs && *this == rhs; }
