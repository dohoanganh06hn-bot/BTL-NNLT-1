#ifndef DATE_UTILS_H
#define DATE_UTILS_H

#include <string>

std::string getCurrentDate();        // Lấy ngày hiện tại
std::string getDueDate(int days=30); // Ngày trả, mặc định 30 ngày
std::string getTomorrowDate();       // Ngày mai
int getDaysBetween(const std::string &from, const std::string &to); // số ngày giữa 2 ngày

#endif
