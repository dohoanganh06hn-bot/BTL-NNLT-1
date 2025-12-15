#include "dateUtils.h"
#include <ctime>
#include <sstream>
#include <iomanip>

std::string getCurrentDate() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    std::stringstream ss;
    ss << (now->tm_year + 1900) << "-" 
       << std::setw(2) << std::setfill('0') << (now->tm_mon + 1) << "-" 
       << std::setw(2) << std::setfill('0') << now->tm_mday;
    return ss.str();
}

std::string getDueDate(int days) {
    time_t t = time(nullptr);
    t += days * 24 * 3600;
    tm* due = localtime(&t);
    std::stringstream ss;
    ss << (due->tm_year + 1900) << "-" 
       << std::setw(2) << std::setfill('0') << (due->tm_mon + 1) << "-" 
       << std::setw(2) << std::setfill('0') << due->tm_mday;
    return ss.str();
}

std::string getTomorrowDate() {
    return getDueDate(1);
}

// đơn giản dùng hàm diff ngày, không xét giờ
int getDaysBetween(const std::string &from, const std::string &to) {
    int y1,m1,d1,y2,m2,d2;
    sscanf(from.c_str(), "%d-%d-%d", &y1,&m1,&d1);
    sscanf(to.c_str(), "%d-%d-%d", &y2,&m2,&d2);

    tm a={0,0,0,d1,m1-1,y1-1900};
    tm b={0,0,0,d2,m2-1,y2-1900};
    time_t t1 = mktime(&a);
    time_t t2 = mktime(&b);
    return (int)((t2 - t1)/86400);
}
