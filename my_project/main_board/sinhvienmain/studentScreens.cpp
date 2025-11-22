#include "studentScreens.h"
#include "book.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <algorithm>

using namespace std;

// -------------------- Hàm tiện ích --------------------

// Lấy ngày hiện tại
string getCurrentDate()
{
    time_t t = time(nullptr);
    tm *now = localtime(&t);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d", now);
    return string(buf);
}

// Lấy ngày hôm sau
string getTomorrowDate()
{
    time_t t = time(nullptr) + 24 * 60 * 60;
    tm *tm_tomorrow = localtime(&t);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d", tm_tomorrow);
    return string(buf);
}

// Tính ngày hết hạn (30 ngày)
string getDueDate()
{
    time_t t = time(nullptr);
    t += 30 * 24 * 60 * 60;
    tm *due = localtime(&t);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d", due);
    return string(buf);
}

// -------------------- In danh sách sách --------------------
void printBooks(const vector<Book> &books)
{
    cout << left << setw(5) << "ID"
         << setw(25) << "Tên sách"
         << setw(15) << "Tác giả"
         << setw(6) << "Còn"
         << setw(8) << "Đang mượn"
         << setw(10) << "Thể loại"
         << setw(10) << "Số lần mượn" << "\n";
    cout << "---------------------------------------------------------------\n";

    for (const auto &b : books)
    {
        cout << left << setw(5) << b.id
             << setw(25) << b.ten
             << setw(15) << b.tac_gia
             << setw(6) << (b.number - b.so_lan_dang_muon)
             << setw(8) << b.so_lan_dang_muon
             << setw(10) << b.the_loai
             << setw(10) << b.so_lan_muon
             << "\n";
    }
}

// =================== Màn hình 3: Tìm & đặt sách ===================
void screen3_borrowBookStudent(sqlite3 *DB, sinhvien &sv)
{
    while (true)
    {
        cout << "\n=== LibBot: Tìm & Đặt sách ===\n";
        cout << "1. Tìm sách theo tên\n";
        cout << "2. Tìm sách theo tác giả\n";
        cout << "3. Tìm sách theo thể loại\n";
        cout << "4. Xem toàn bộ sách\n";
        cout << "5. Đặt trước sách (nếu hết)\n";
        cout << "0. Thoát\n";
        cout << "Chọn: ";

        int opt;
        cin >> opt;
        cin.ignore();
        if (opt == 0)
            break;

        if (opt == 1)
        {
            cout << "Nhập từ khóa tên sách: ";
            string keyword;
            getline(cin, keyword);
            vector<Book> books = Book::searchByName(DB, keyword);
            printBooks(books);
        }
        else if (opt == 2)
        {
            cout << "Nhập từ khóa tác giả: ";
            string keyword;
            getline(cin, keyword);
            vector<Book> books = Book::searchByAuthor(DB, keyword);
            printBooks(books);
        }
        else if (opt == 3)
        {
            cout << "Nhập các số thể loại (1,3,5) cách nhau bằng dấu ,: ";
            string input;
            getline(cin, input);
            vector<int> categories;
            stringstream ss(input);
            string item;
            while (getline(ss, item, ','))
                categories.push_back(stoi(item));
            vector<Book> books = Book::searchByCategory(DB, categories);
            printBooks(books);
        }
        else if (opt == 4)
        {
            vector<Book> books = Book::getAllBooks(DB);
            printBooks(books);
        }
        else if (opt == 5)
        {
            int book_id;
            cout << "Nhập ID sách muốn đặt: ";
            cin >> book_id;
            cin.ignore();

            // Kiểm tra sách còn hay không
            vector<Book> books = Book::searchByCategory(DB, {}); // tất cả sách
            auto it = find_if(books.begin(), books.end(), [&](const Book &b)
                              { return b.id == book_id; });
            if (it == books.end())
            {
                cout << "Sách không tồn tại!\n";
                continue;
            }

            if (it->so_lan_dang_muon < it->number)
            {
                cout << "Sách còn, không cần đặt trước!\n";
                continue;
            }

            if (Book::preorderBook(DB, sv.id, book_id))
            {
                ofstream logFile("mail_log.txt", ios::app);
                logFile << sv.id << "," << book_id << "," << getCurrentDate() << ",reserved\n";
                logFile.close();
                cout << "Đặt trước thành công! Bạn sẽ nhận thông báo khi sách về.\n";
            }
            else
            {
                cout << "Đặt trước thất bại!\n";
            }
        }
        else
        {
            cout << "Lựa chọn không hợp lệ!\n";
        }

        cout << "\nẤn Enter để tiếp tục...";
        cin.ignore();
        cin.get();
    }
}

// =================== Màn hình 4: Hồ sơ học sinh ===================
void screen4_profileStudent(sqlite3 *DB, sinhvien &sv)
{
    cout << "\n=== Hồ sơ học sinh ===\n";
    cout << "ID: " << sv.id << "\nTên: " << sv.ten << "\nEmail: " << sv.email << "\n";

    // Hiển thị sách đang mượn
    sqlite3_stmt *stmt;
    string sql = "SELECT b.id, b.ten, m.ngay_muon, m.ngay_phai_tra FROM muonsach m "
                 "JOIN book b ON m.id_book = b.id "
                 "WHERE m.id_sinhvien = ? AND m.trang_thai='borrowing';";
    sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, sv.id);

    cout << "\nSách đang mượn:\n";
    cout << left << setw(5) << "ID" << setw(25) << "Tên sách"
         << setw(12) << "Ngày mượn" << setw(12) << "Hạn trả" << "\n";
    cout << "------------------------------------------------\n";

    bool hasBooks = false;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        hasBooks = true;
        int book_id = sqlite3_column_int(stmt, 0);
        string book_name = (const char *)sqlite3_column_text(stmt, 1);
        string borrow_date = (const char *)sqlite3_column_text(stmt, 2);
        string due_date = (const char *)sqlite3_column_text(stmt, 3);
        cout << left << setw(5) << book_id << setw(25) << book_name
             << setw(12) << borrow_date << setw(12) << due_date << "\n";

        // Tự động gửi mail nhắc trước 1 ngày (ghi log)
        if (due_date == getTomorrowDate())
        {
            ofstream logFile("mail_log.txt", ios::app);
            logFile << sv.id << "," << book_id << "," << borrow_date << "," << due_date << ",remind\n";
            logFile.close();
        }
    }
    if (!hasBooks)
        cout << "Chưa mượn sách nào.\n";
    sqlite3_finalize(stmt);
    cout << "\nẤn Enter để quay lại menu...";
    cin.ignore();
    cin.get();
}

// =================== Main Menu sinh viên ===================
void mainMenuStudent(sqlite3 *DB, sinhvien &sv)
{
    int choice;
    do
    {
        cout << "\n=== Main Menu Học sinh ===\n";
        cout << "1. Mượn sách (LibBot)\n";
        cout << "2. Hồ sơ\n";
        cout << "0. Thoát\n";
        cout << "Chọn: ";
        cin >> choice;
        cin.ignore();
        switch (choice)
        {
        case 1:
            screen3_borrowBookStudent(DB, sv);
            break;
        case 2:
            screen4_profileStudent(DB, sv);
            break;
        case 0:
            cout << "Thoát...\n";
            break;
        default:
            cout << "Lựa chọn không hợp lệ.\n";
        }
    } while (choice != 0);
}
