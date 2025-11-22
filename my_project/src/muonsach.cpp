#include "muon_sach.h"
#include <vector>
#include <string>
#include <iostream>

bool muonsach::addToDB(sqlite3 *DB)
{
    const char *sql = "INSERT INTO muonsach (id_sinhvien, id_book, ngay_muon, ngay_phai_tra, trang_thai) "
                      "VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Prepare failed: " << sqlite3_errmsg(DB) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, id_sinhvien);
    sqlite3_bind_int(stmt, 2, id_book);
    sqlite3_bind_text(stmt, 3, ngay_muon.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, ngay_phai_tra.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, trang_thai.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        std::cerr << "Insert failed: " << sqlite3_errmsg(DB) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}
std::vector<muonsach> muonsach::getBorrowingList(sqlite3 *DB, int student_id)
{
    std::vector<muonsach> list;
    sqlite3_stmt *stmt;

    std::string sql =
        "SELECT id, student_id, book_id, borrow_date, due_date "
        "FROM muonsach WHERE student_id = ? AND status = 'borrowing';";

    sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, student_id);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        muonsach m;
        m.id = sqlite3_column_int(stmt, 0);
        m.student_id = sqlite3_column_int(stmt, 1);
        m.book_id = sqlite3_column_int(stmt, 2);
        m.borrow_date = (const char *)sqlite3_column_text(stmt, 3);
        m.due_date = (const char *)sqlite3_column_text(stmt, 4);
        list.push_back(m);
    }
    sqlite3_finalize(stmt);
    return list;
}
bool Book::decreaseBorrowing(sqlite3 *DB, int book_id)
{
    const char *sql = "UPDATE book SET so_lan_dang_muon = so_lan_dang_muon - 1 WHERE id = ? AND so_lan_dang_muon > 0;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int(stmt, 1, book_id);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}
