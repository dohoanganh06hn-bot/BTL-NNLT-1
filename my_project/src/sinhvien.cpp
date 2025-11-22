#include "sinhvien.h"
#include <iostream>

bool sinhvien::addsinhvien(sqlite3 *DB, const sinhvien &s)
{
    const char *sql = "INSERT INTO sinhvien(ten,email,password) VALUES(?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Prepare failed: " << sqlite3_errmsg(DB) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, s.ten.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, s.email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, s.password.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        std::cerr << "Insert failed: " << sqlite3_errmsg(DB) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

sinhvien *sinhvien::login(sqlite3 *DB, const std::string &email, const std::string &password)
{
    const char *sql = "SELECT id, ten, email, password FROM sinhvien WHERE email=? AND password=?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Prepare failed: " << sqlite3_errmsg(DB) << std::endl;
        return nullptr;
    }

    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);

    sinhvien *sv = nullptr;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        std::string ten = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        std::string email_ret = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        std::string pass_ret = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));

        sv = new sinhvien(id, ten, email_ret, pass_ret);
    }

    sqlite3_finalize(stmt);
    return sv; // Nếu nullptr -> login thất bại
}
int sinhvien::getBorrowingCount(sqlite3 *DB, int student_id)
{
    const char *sql = "SELECT COUNT(*) FROM muonsach WHERE id_sinhvien=? AND trang_thai='borrowing';";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return 0;

    sqlite3_bind_int(stmt, 1, student_id);
    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        count = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return count;
}
