#pragma once

#include <chrono>
#include <fstream>
#include <iostream>
#include <time.h>

class Timestamp {
    public:
        Timestamp();
        Timestamp(bool future);
        Timestamp(long unixtime);
        Timestamp(const char datestr[]);
        Timestamp(std::string datestr);

        bool operator==(const Timestamp& rhs) const;
        bool operator!=(const Timestamp& rhs) const;
        // need to add more operators

        static long current_unixtime();

        std::string get_str(bool truncate) const;
        std::string get_str() const;
        long get_unixtime() const;

        bool is_future() const;
        bool is_zero() const;
        int get_year() const;
        int get_month() const;
        int get_date() const;
        int get_day() const;
        int get_hour() const;
        int get_min() const;

    private:
        void InitFuturetime();
        void TmFromUnix(long unixtime);
        void TmFromStr(std::string datestr);
        struct tm tm;
        static long futuretime;
};
