#include "Book.h"
#include <sqlite3.h>
#include <vector>
#include <string>
#include <ctime>
#include <iostream>
#include <iomanip>


void Book::printBooks(const std::vector<Book>& books) {
    std::cout << std::left << std::setw(5) << "ID"
              << std::setw(25) << "Tên sách"
              << std::setw(15) << "Tác giả"
              << std::setw(6) << "Còn"
              << std::setw(8) << "Đang mượn"
              << std::setw(10) << "Thể loại"
              << std::setw(10) << "Số lần mượn" << "\n";
    std::cout << "---------------------------------------------------------------\n";
    for (const auto &b : books) {
        std::cout << std::left << std::setw(5) << b.id
                  << std::setw(25) << b.ten
                  << std::setw(15) << b.tac_gia
                  << std::setw(6) << (b.number - b.so_lan_dang_muon)
                  << std::setw(8) << b.so_lan_dang_muon
                  << std::setw(10) << b.the_loai
                  << std::setw(10) << b.so_lan_muon
                  << "\n";
    }
}


// -------------------- Thêm sách --------------------
bool Book::addBook(sqlite3 *DB, const Book &b)
{
    std::string sql = "INSERT INTO book(ten, tac_gia, year, tai_ban, number, so_lan_muon, so_lan_dang_muon, the_loai) VALUES ('" + b.ten + "', '" + b.tac_gia + "', " + std::to_string(b.year) + ", '" + b.tai_ban + "', " + std::to_string(b.number) + ", 0, 0, '" + b.the_loai + "');";
    return sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK;
}

// -------------------- Lấy sách theo ID --------------------
Book Book::getById(sqlite3 *DB, int book_id)
{
    Book b;
    sqlite3_stmt *stmt;
    std::string sql = "SELECT * FROM book WHERE id=" + std::to_string(book_id) + ";";
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        return b;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        b.id = sqlite3_column_int(stmt, 0);
        b.ten = (const char *)sqlite3_column_text(stmt, 1);
        b.tac_gia = (const char *)sqlite3_column_text(stmt, 2);
        b.year = sqlite3_column_int(stmt, 3);
        b.tai_ban = (const char *)sqlite3_column_text(stmt, 4);
        b.number = sqlite3_column_int(stmt, 5);
        b.so_lan_muon = sqlite3_column_int(stmt, 6);
        b.so_lan_dang_muon = sqlite3_column_int(stmt, 7);
        b.the_loai = (const char *)sqlite3_column_text(stmt, 8);
    }

    sqlite3_finalize(stmt);
    return b;
}

// -------------------- Cập nhật sau khi mượn --------------------
bool Book::updateAfterBorrow(sqlite3 *DB, int book_id)
{
    std::string sql = "UPDATE book SET so_lan_muon=so_lan_muon+1, so_lan_dang_muon=so_lan_dang_muon+1 WHERE id=" + std::to_string(book_id) + ";";
    return sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK;
}

// -------------------- Giảm số sách đang mượn --------------------
bool Book::decreaseBorrowing(sqlite3 *DB, int book_id)
{
    std::string sql = "UPDATE book SET so_lan_dang_muon=so_lan_dang_muon-1 WHERE id=" + std::to_string(book_id) + " AND so_lan_dang_muon>0;";
    return sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK;
}

// -------------------- Truy vấn --------------------
std::vector<Book> Book::searchByName(sqlite3 *DB, const std::string &keyword)
{
    std::vector<Book> results;
    std::string sql = "SELECT * FROM book WHERE ten LIKE '%" + keyword + "%';";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        return results;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        Book b;
        b.id = sqlite3_column_int(stmt, 0);
        b.ten = (const char *)sqlite3_column_text(stmt, 1);
        b.tac_gia = (const char *)sqlite3_column_text(stmt, 2);
        b.year = sqlite3_column_int(stmt, 3);
        b.tai_ban = (const char *)sqlite3_column_text(stmt, 4);
        b.number = sqlite3_column_int(stmt, 5);
        b.so_lan_muon = sqlite3_column_int(stmt, 6);
        b.so_lan_dang_muon = sqlite3_column_int(stmt, 7);
        b.the_loai = (const char *)sqlite3_column_text(stmt, 8);
        results.push_back(b);
    }

    sqlite3_finalize(stmt);
    return results;
}

std::vector<Book> Book::searchByAuthor(sqlite3 *DB, const std::string &keyword)
{
    std::vector<Book> results;
    std::string sql = "SELECT * FROM book WHERE tac_gia LIKE '%" + keyword + "%';";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        return results;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        Book b;
        b.id = sqlite3_column_int(stmt, 0);
        b.ten = (const char *)sqlite3_column_text(stmt, 1);
        b.tac_gia = (const char *)sqlite3_column_text(stmt, 2);
        b.year = sqlite3_column_int(stmt, 3);
        b.tai_ban = (const char *)sqlite3_column_text(stmt, 4);
        b.number = sqlite3_column_int(stmt, 5);
        b.so_lan_muon = sqlite3_column_int(stmt, 6);
        b.so_lan_dang_muon = sqlite3_column_int(stmt, 7);
        b.the_loai = (const char *)sqlite3_column_text(stmt, 8);
        results.push_back(b);
    }

    sqlite3_finalize(stmt);
    return results;
}

std::vector<Book> Book::getAllBooks(sqlite3 *DB)
{
    std::vector<Book> results;
    std::string sql = "SELECT * FROM book;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        return results;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        Book b;
        b.id = sqlite3_column_int(stmt, 0);
        b.ten = (const char *)sqlite3_column_text(stmt, 1);
        b.tac_gia = (const char *)sqlite3_column_text(stmt, 2);
        b.year = sqlite3_column_int(stmt, 3);
        b.tai_ban = (const char *)sqlite3_column_text(stmt, 4);
        b.number = sqlite3_column_int(stmt, 5);
        b.so_lan_muon = sqlite3_column_int(stmt, 6);
        b.so_lan_dang_muon = sqlite3_column_int(stmt, 7);
        b.the_loai = (const char *)sqlite3_column_text(stmt, 8);
        results.push_back(b);
    }

    sqlite3_finalize(stmt);
    return results;
}

// -------------------- Preorder --------------------
bool Book::preorderBook(sqlite3 *DB, int student_id, int book_id)
{
    time_t t = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&t));

    std::string sql = "INSERT INTO preorder(id_sinhvien, id_book, request_date) VALUES (" + std::to_string(student_id) + ", " + std::to_string(book_id) + ", '" + std::string(buf) + "');";

    return sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK;
}

std::vector<Book> Book::searchByCategory(sqlite3 *DB, const std::vector<int> &categories) {
    std::vector<Book> results;
    if(categories.empty()) return results;

    std::string sql = "SELECT * FROM book WHERE the_loai IN (";
    for(size_t i=0; i<categories.size(); i++) {
        sql += std::to_string(categories[i]);
        if(i < categories.size()-1) sql += ",";
    }
    sql += ");";

    sqlite3_stmt *stmt;
    if(sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        return results;

    while(sqlite3_step(stmt) == SQLITE_ROW) {
        Book b;
        b.id = sqlite3_column_int(stmt, 0);
        b.ten = (const char*)sqlite3_column_text(stmt, 1);
        b.tac_gia = (const char*)sqlite3_column_text(stmt, 2);
        b.year = sqlite3_column_int(stmt, 3);
        b.tai_ban = (const char*)sqlite3_column_text(stmt, 4);
        b.number = sqlite3_column_int(stmt, 5);
        b.so_lan_muon = sqlite3_column_int(stmt, 6);
        b.so_lan_dang_muon = sqlite3_column_int(stmt, 7);
        b.the_loai = (const char*)sqlite3_column_text(stmt, 8);
        results.push_back(b);
    }

    sqlite3_finalize(stmt);
    return results;
}
