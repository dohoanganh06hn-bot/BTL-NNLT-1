#ifndef MENU_MANAGEMENT_H
#define MENU_MANAGEMENT_H 

#include <sqlite3.h>
#include <string> 
void mainMenuManagement(sqlite3* DB, const std::string& role);

void screen3_addBook(sqlite3* DB);
void screen4_listBooks(sqlite3* DB);
void screen5_borrowBook(sqlite3* DB);
void screen6_returnBook(sqlite3* DB);
void screen7_expiringBooks(sqlite3* DB);

#endif