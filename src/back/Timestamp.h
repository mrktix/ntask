#pragma once

#include <fstream>
#include <time.h>
#include <chrono>

class Timestamp {
    public:
        Timestamp();
        Timestamp(bool future);
        Timestamp(long unixtime);
        Timestamp(std::string datestr);

        bool operator==(const Timestamp& rhs) const;
        bool operator!=(const Timestamp& rhs) const;

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
        struct tm tm;
        static long futuretime;
};
