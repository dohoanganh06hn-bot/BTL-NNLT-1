#include "dateUtils.h"
#include <ctime>

std::string getCurrentDate() {
    time_t t = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&t));
    return buf;
}

std::string getTomorrowDate() {
    time_t t = time(nullptr) + 24*3600;
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&t));
    return buf;
}

std::string getDueDate() {
    time_t t = time(nullptr) + 7*24*3600; // ví dụ 7 ngày
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&t));
    return buf;
}
