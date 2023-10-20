#include "Timestamp.h"

long Timestamp::futuretime = 0;

Timestamp::Timestamp(long unixtime) {
    if (futuretime == 0) futuretime = 0;
}

long Timestamp::current_unixtime() const {
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
