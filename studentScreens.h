#ifndef STUDENT_SCREENS_H
#define STUDENT_SCREENS_H

#include <string>
#include "sinhvien.h"
#include <sqlite3.h>

// Menu chính
void mainMenuStudent(sqlite3* DB, sinhvien &sv);
void screen3_borrowBookStudent(sqlite3* DB, sinhvien &sv);
void screen4_profileStudent(sqlite3* DB, sinhvien &sv);

// Các hàm tiện ích
std::string getCurrentDate();
std::string getTomorrowDate();
std::string getDueDate();

#endif
