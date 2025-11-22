#include "book.h"
#include <iostream>

using namespace std;

// -------------------- Thêm sách (dùng bind để tránh SQL injection) --------------------
bool Book::addBook(sqlite3 *DB, const Book &b)
{
    const char *sql = "INSERT INTO book(ten, tac_gia, year, tai_ban, number, so_lan_muon, so_lan_dang_muon, the_loai) "
                      "VALUES(?, ?, ?, ?, ?, 0, 0, ?);";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(DB, sql, -1, &stmt, 0) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, b.ten.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, b.tac_gia.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, b.year);
    sqlite3_bind_text(stmt, 4, b.tai_ban.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, b.number);
    sqlite3_bind_text(stmt, 6, b.the_loai.c_str(), -1, SQLITE_TRANSIENT);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

// -------------------- Hàm helper đọc text an toàn --------------------
inline std::string safeText(const unsigned char *text)
{
    return text ? std::string(reinterpret_cast<const char *>(text)) : "";
}

// -------------------- Truy vấn sách --------------------
vector<Book> Book::searchByName(sqlite3 *DB, const string &keyword)
{
    vector<Book> results;
    string sql = "SELECT * FROM book WHERE ten LIKE ?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0) != SQLITE_OK)
        return results;

    string likePattern = "%" + keyword + "%";
    sqlite3_bind_text(stmt, 1, likePattern.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        Book b;
        b.id = sqlite3_column_int(stmt, 0);
        b.ten = safeText(sqlite3_column_text(stmt, 1));
        b.tac_gia = safeText(sqlite3_column_text(stmt, 2));
        b.year = sqlite3_column_int(stmt, 3);
        b.tai_ban = safeText(sqlite3_column_text(stmt, 4));
        b.number = sqlite3_column_int(stmt, 5);
        b.so_lan_muon = sqlite3_column_int(stmt, 6);
        b.so_lan_dang_muon = sqlite3_column_int(stmt, 7);
        b.the_loai = safeText(sqlite3_column_text(stmt, 8));
        results.push_back(b);
    }
    sqlite3_finalize(stmt);
    return results;
}

vector<Book> Book::searchByAuthor(sqlite3 *DB, const string &keyword)
{
    vector<Book> results;
    string sql = "SELECT * FROM book WHERE tac_gia LIKE ?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0) != SQLITE_OK)
        return results;

    string likePattern = "%" + keyword + "%";
    sqlite3_bind_text(stmt, 1, likePattern.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        Book b;
        b.id = sqlite3_column_int(stmt, 0);
        b.ten = safeText(sqlite3_column_text(stmt, 1));
        b.tac_gia = safeText(sqlite3_column_text(stmt, 2));
        b.year = sqlite3_column_int(stmt, 3);
        b.tai_ban = safeText(sqlite3_column_text(stmt, 4));
        b.number = sqlite3_column_int(stmt, 5);
        b.so_lan_muon = sqlite3_column_int(stmt, 6);
        b.so_lan_dang_muon = sqlite3_column_int(stmt, 7);
        b.the_loai = safeText(sqlite3_column_text(stmt, 8));
        results.push_back(b);
    }
    sqlite3_finalize(stmt);
    return results;
}

vector<Book> Book::searchByCategory(sqlite3 *DB, const vector<int> &categories)
{
    vector<Book> results;
    if (categories.empty())
        return results;

    string sql = "SELECT * FROM book WHERE ";
    for (size_t i = 0; i < categories.size(); ++i)
    {
        if (i > 0)
            sql += " OR ";
        sql += "the_loai LIKE ?";
    }
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0) != SQLITE_OK)
        return results;

    for (size_t i = 0; i < categories.size(); ++i)
    {
        string pattern = "%" + to_string(categories[i]) + "%";
        sqlite3_bind_text(stmt, static_cast<int>(i + 1), pattern.c_str(), -1, SQLITE_TRANSIENT);
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        Book b;
        b.id = sqlite3_column_int(stmt, 0);
        b.ten = safeText(sqlite3_column_text(stmt, 1));
        b.tac_gia = safeText(sqlite3_column_text(stmt, 2));
        b.year = sqlite3_column_int(stmt, 3);
        b.tai_ban = safeText(sqlite3_column_text(stmt, 4));
        b.number = sqlite3_column_int(stmt, 5);
        b.so_lan_muon = sqlite3_column_int(stmt, 6);
        b.so_lan_dang_muon = sqlite3_column_int(stmt, 7);
        b.the_loai = safeText(sqlite3_column_text(stmt, 8));
        results.push_back(b);
    }
    sqlite3_finalize(stmt);
    return results;
}

vector<Book> Book::getAllBooks(sqlite3 *DB)
{
    vector<Book> results;
    const char *sql = "SELECT * FROM book;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(DB, sql, -1, &stmt, 0) != SQLITE_OK)
        return results;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        Book b;
        b.id = sqlite3_column_int(stmt, 0);
        b.ten = safeText(sqlite3_column_text(stmt, 1));
        b.tac_gia = safeText(sqlite3_column_text(stmt, 2));
        b.year = sqlite3_column_int(stmt, 3);
        b.tai_ban = safeText(sqlite3_column_text(stmt, 4));
        b.number = sqlite3_column_int(stmt, 5);
        b.so_lan_muon = sqlite3_column_int(stmt, 6);
        b.so_lan_dang_muon = sqlite3_column_int(stmt, 7);
        b.the_loai = safeText(sqlite3_column_text(stmt, 8));
        results.push_back(b);
    }
    sqlite3_finalize(stmt);
    return results;
}

// -------------------- Đặt trước sách --------------------
bool Book::preorderBook(sqlite3 *DB, int student_id, int book_id)
{
    const char *sql = "INSERT INTO preorder(student_id, book_id, request_date) VALUES(?, ?, ?);";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(DB, sql, -1, &stmt, 0) != SQLITE_OK)
        return false;

    time_t t = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&t));

    sqlite3_bind_int(stmt, 1, student_id);
    sqlite3_bind_int(stmt, 2, book_id);
    sqlite3_bind_text(stmt, 3, buf, -1, SQLITE_TRANSIENT);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}
Book Book::getById(sqlite3 *DB, int book_id)
{
    Book b;
    const char *sql = "SELECT * FROM book WHERE id=?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return b;

    sqlite3_bind_int(stmt, 1, book_id);
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        b.id = sqlite3_column_int(stmt, 0);
        b.ten = safeText(sqlite3_column_text(stmt, 1));
        b.tac_gia = safeText(sqlite3_column_text(stmt, 2));
        b.year = sqlite3_column_int(stmt, 3);
        b.tai_ban = safeText(sqlite3_column_text(stmt, 4));
        b.number = sqlite3_column_int(stmt, 5);
        b.so_lan_muon = sqlite3_column_int(stmt, 6);
        b.so_lan_dang_muon = sqlite3_column_int(stmt, 7);
        b.the_loai = safeText(sqlite3_column_text(stmt, 8));
    }
    sqlite3_finalize(stmt);
    return b; // nếu id=0 -> sách không tồn tại
}

bool Book::updateAfterBorrow(sqlite3 *DB, int book_id)
{
    const char *sql = "UPDATE book SET so_lan_muon = so_lan_muon+1, so_lan_dang_muon = so_lan_dang_muon+1 WHERE id=?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int(stmt, 1, book_id);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}
