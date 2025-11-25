#include "nhanvien.h"
#include <iostream>

bool nhanvien::addnhanvien(sqlite3 *DB, const nhanvien &emp)
{
    std::string sql = "INSERT INTO nhanvien(ten, password) VALUES('" + emp.ten + "', '" + emp.password + "');";

    char *errMsg = nullptr;
    if (sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        std::cerr << "[Lỗi] Không thêm được nhân viên: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool nhanvien::login(sqlite3 *DB, const std::string &ten, const std::string &pass)
{
    std::string sql = "SELECT id FROM nhanvien WHERE ten='" + ten + "' AND password='" + pass + "';";

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    bool result = (sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);

    return result;
}
