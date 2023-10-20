#pragma once

#include <fstream>
#include <time.h>
#include <chrono>

class Timestamp {
    public:
        Timestamp(long unixtime);
        Timestamp(std::string datestr);

        long get_unixtime() const;
        int get_year() const;
        int get_month() const;
        int get_date() const;
        int get_day() const;

    private:
        long current_unixtime() const;
        struct tm tm;
        static long futuretime;
};
