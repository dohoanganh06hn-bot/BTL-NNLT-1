#ifndef nhanvien_h
#define nhanvien_h

#include <string>
#include <sqlite3.h>

class nhanvien
{
public:
    int id;
    std::string ten;
    std::string password;

    nhanvien() {}
    nhanvien(std::string n, std::string p) : ten(n), password(p) {}
    static bool addnhanvien(sqlite3 *DB, const nhanvien &emp);
    static bool login(sqlite3 *DB, const std::string &ten, const std::string &pass);
};

#endif
