#include "Timestamp.h"

long Timestamp::futuretime = 0;
std::string Timestamp::zero_datestr = "0000.01.01*00:00";

void Timestamp::TmFromUnix(long unixtime) {
    tm = *std::localtime(&unixtime);

    tm.tm_isdst = -1;
    tm.tm_year += 1900;
    tm.tm_mon += 1;
    tm.tm_sec = 0;
}

std::string Timestamp::untruncate(std::string datestr) {
    return datestr + zero_datestr.substr(datestr.length());
}

std::vector<int> Timestamp::parse_datestr(std::string datestr) {
    std::vector<int> ret = {0, 0, 0, 0, 0};

    enum stage {
        stage_year, stage_month, stage_day, stage_hour, stage_min,
    } parsing_stage = stage_year;

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
                case stage_year:  ret[0] *= 10; ret[0] += ch - '0'; break;
                case stage_month: ret[1] *= 10; ret[1] += ch - '0'; break;
                case stage_day:   ret[2] *= 10; ret[2] += ch - '0'; break;
                case stage_hour:  ret[3] *= 10; ret[3] += ch - '0'; break;
                case stage_min:   ret[4] *= 10; ret[4] += ch - '0'; break;
            }
        }
    }

    return ret;
}

void Timestamp::TmFromStr(std::string datestr) {
    if (datestr.empty()) { TmFromUnix(0); return; }

    bool additive_date;
    if (datestr[0] == '+') {
        additive_date = true;
        datestr = "0000.00."+datestr.substr(1);
    } else additive_date = false;
    datestr = untruncate(datestr);

    std::vector<int> parsed_date = parse_datestr(datestr);

    if (additive_date) { // does not support months and years
        long utime = 0;
        utime += 60*parsed_date[4]; // minutes
        utime += 60*60*parsed_date[3]; // hours
        utime += 24*60*60*parsed_date[2]; // days
        // ignore months & years
        TmFromUnix(utime);
    } else {
        tm.tm_year = parsed_date[0] - 1900;
        tm.tm_mon  = parsed_date[1] - 1;
        tm.tm_mday = parsed_date[2];
        tm.tm_hour = parsed_date[3];
        tm.tm_min  = parsed_date[4];
        tm.tm_sec = 0;
        tm.tm_isdst = -1;
        TmFromUnix(std::mktime(&tm));
    }
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

Timestamp Timestamp::operator+(const Timestamp &rhs) const {
    return Timestamp(get_unixtime() + rhs.get_unixtime());
}

bool Timestamp::operator==(const Timestamp& rhs) const {
    return get_unixtime() == rhs.get_unixtime();
}
bool Timestamp::operator<(const Timestamp& rhs) const {
    return get_unixtime() < rhs.get_unixtime();
}

bool Timestamp::operator!=(const Timestamp& rhs) const { return !(*this == rhs); }
bool Timestamp::operator>=(const Timestamp& rhs) const { return !(*this < rhs); }
bool Timestamp::operator>(const Timestamp& rhs) const { return !(*this < rhs || *this == rhs); }
bool Timestamp::operator<=(const Timestamp& rhs) const { return *this < rhs || *this == rhs; }
