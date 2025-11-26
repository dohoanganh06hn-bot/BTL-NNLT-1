#include "muon_sach.h"
#include <vector>
#include <string>
#include <ctime>

// -------------------- Thêm bản ghi mượn sách --------------------
bool muonsach::addToDB(sqlite3 *DB)
{
    std::string sql = "INSERT INTO muonsach (id_sinhvien, id_book, ngay_muon, ngay_phai_tra, trang_thai) VALUES (" + std::to_string(id_sinhvien) + ", " + std::to_string(id_book) + ", '" + ngay_muon + "', '" + ngay_phai_tra + "', '" + trang_thai + "');";

    return sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK;
}

// -------------------- Danh sách sách đang mượn của sinh viên --------------------
std::vector<muonsach> muonsach::getBorrowingList(sqlite3 *DB, int student_id)
{
    std::vector<muonsach> list;
    std::string sql = "SELECT id, id_sinhvien, id_book, ngay_muon, ngay_phai_tra, trang_thai FROM muonsach "
                      "WHERE id_sinhvien=" +
                      std::to_string(student_id) + " AND trang_thai='dang muon';";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        return list;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        muonsach m;
        m.id = sqlite3_column_int(stmt, 0);
        m.id_sinhvien = sqlite3_column_int(stmt, 1);
        m.id_book = sqlite3_column_int(stmt, 2);
        m.ngay_muon = (const char *)sqlite3_column_text(stmt, 3);
        m.ngay_phai_tra = (const char *)sqlite3_column_text(stmt, 4);
        m.trang_thai = (const char *)sqlite3_column_text(stmt, 5);
        list.push_back(m);
    }

    sqlite3_finalize(stmt);
    return list;
}

// -------------------- Trả sách --------------------
bool muonsach::returnBook(sqlite3 *DB, int student_id, int book_id)
{
    std::string sql = "UPDATE muonsach SET trang_thai='da tra' WHERE id_sinhvien=" + std::to_string(student_id) + " AND id_book=" + std::to_string(book_id) + " AND trang_thai='dang muon';";
    return sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK;
}

// -------------------- Danh sách đặt trước --------------------
std::vector<int> muonsach::getPreorderList(sqlite3 *DB, int book_id)
{
    std::vector<int> students;
    std::string sql = "SELECT id_sinhvien FROM preorder WHERE id_book=" + std::to_string(book_id) + ";";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        return students;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        students.push_back(sqlite3_column_int(stmt, 0));
    }

    sqlite3_finalize(stmt);
    return students;
}
