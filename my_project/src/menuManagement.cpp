#include "menuManagement.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <time.h>
#include "book.h"
#include "sinhvien.h"
#include "muon_sach.h"
#include <algorithm>
#include "muon_sach.h"
using namespace std;

string getTomorrowDate()
{
    time_t t = time(nullptr);
    t += 24 * 60 * 60; // + 1 ngày
    tm *tomorrow = localtime(&t);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d", tomorrow);
    return string(buf);
}

string getCurrentDate()
{
    time_t t = time(nullptr);
    tm *now = localtime(&t);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d", now);
    return string(buf);
}

string getDueDate()
{
    time_t t = time(nullptr);
    t += 30 * 24 * 60 * 60;
    tm *due = localtime(&t);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d", due);
    return string(buf);
}

void screen3_addBook(sqlite3 *DB)
{
    cin.ignore(); // xóa buffer
    string ten, tac_gia, tai_ban, the_loai;
    int year, number;

    cout << "\n=== Thêm sách mới ===\n";
    cout << "Tên sách: ";
    getline(cin, ten);
    cout << "Tác giả: ";
    getline(cin, tac_gia);
    cout << "Năm xuất bản: ";
    cin >> year;
    cin.ignore();
    cout << "Tái bản: ";
    getline(cin, tai_ban);
    cout << "Số lượng: ";
    cin >> number;
    cin.ignore();
    cout << "Thể loại (có thể nhập nhiều, cách nhau bằng dấu ,): ";
    getline(cin, the_loai);

    // Tạo object Book
    Book b(ten, tac_gia, year, tai_ban, number, the_loai);

    // Thêm sách qua hàm class
    if (Book::addBook(DB, b))
    {
        cout << "Thêm sách thành công!\n";
    }
    else
    {
        cerr << "Lỗi thêm sách!\n";
    }

    cout << "\nẤn Enter để quay lại menu...";
    cin.get();
}

// In danh sách sách từ vector<Book>
void printBooks(const std::vector<Book> &books)
{
    cout << "\n-------------------------------------------------------------\n";
    cout << left << setw(5) << "ID"
         << setw(25) << "Tên sách"
         << setw(15) << "Tác giả"
         << setw(6) << "Năm"
         << setw(8) << "SL"
         << setw(8) << "Đang m."
         << setw(15) << "Thể loại"
         << "\n";
    cout << "-------------------------------------------------------------\n";

    for (const auto &b : books)
    {
        cout << left << setw(5) << b.id
             << setw(25) << b.ten
             << setw(15) << b.tac_gia
             << setw(6) << b.year
             << setw(8) << b.number
             << setw(8) << b.so_lan_dang_muon
             << setw(15) << b.the_loai
             << "\n";
    }

    cout << "-------------------------------------------------------------\n";
}

// Màn hình 4: xem sách
void screen4_listBooks(sqlite3 *DB)
{
    while (true)
    {
        cout << "\n=== Màn hình 4: Xem danh sách sách ===\n";
        cout << "1. Xem toàn bộ sách\n";
        cout << "2. Tìm kiếm sách\n";
        cout << "0. Quay lại menu chính\n";
        cout << "Chọn: ";

        int opt;
        cin >> opt;
        cin.ignore();

        if (opt == 0)
            return;

        std::vector<Book> books;

        if (opt == 1)
        {
            books = Book::getAllBooks(DB);
        }
        else if (opt == 2)
        {
            cout << "Nhập từ khóa (tên, tác giả hoặc thể loại): ";
            string keyword;
            getline(cin, keyword);

            // Có thể combine nhiều cách tìm kiếm
            auto byName = Book::searchByName(DB, keyword);
            auto byAuthor = Book::searchByAuthor(DB, keyword);
            // Mình sẽ bỏ searchByCategory đơn giản, hoặc có thể parse keyword thành vector<int>

            books.insert(books.end(), byName.begin(), byName.end());
            books.insert(books.end(), byAuthor.begin(), byAuthor.end());

            // Loại trùng lặp ID
            std::sort(books.begin(), books.end(), [](const Book &a, const Book &b)
                      { return a.id < b.id; });
            books.erase(std::unique(books.begin(), books.end(), [](const Book &a, const Book &b)
                                    { return a.id == b.id; }),
                        books.end());
        }
        else
        {
            cout << "Lựa chọn không hợp lệ!\n";
            continue;
        }

        printBooks(books);

        cout << "\nẤn Enter để quay lại menu...";
        std::cin.ignore();
        std::cin.get();
    }
}

void screen5_borrowBook(sqlite3 *DB)
{
    int student_id, book_id;
    cout << "\n=== Mượn sách ===\n";
    cout << "Nhập ID sinh viên: ";
    cin >> student_id;
    cout << "Nhập ID sách: ";
    cin >> book_id;

    // 1️⃣ Kiểm tra sách
    Book b = Book::getById(DB, book_id);
    if (b.id == 0)
    {
        cout << "Sách không tồn tại!\n";
        return;
    }

    // 2️⃣ Kiểm tra số sách đang mượn của sinh viên
    int studentBorrowing = sinhvien::getBorrowingCount(DB, student_id);

    if (b.so_lan_dang_muon >= b.number)
    {
        cout << "Sách đã hết. Lưu yêu cầu đặt trước...\n";
        if (Book::preorderBook(DB, student_id, book_id))
        {
            // Ghi log
            ofstream logFile("mail_log.txt", ios::app);
            logFile << student_id << "," << book_id << "," << getCurrentDate() << ",reserved\n";
            logFile.close();
            cout << "Đặt trước thành công!\n";
        }
        return;
    }

    // 3️⃣ Cho mượn sách
    string borrow_date = getCurrentDate();
    string due_date = getDueDate();

    muonsach m(student_id, book_id, borrow_date, due_date, "borrowing");
    if (m.addToDB(DB) && Book::updateAfterBorrow(DB, book_id))
    {
        // Ghi log
        ofstream logFile("mail_log.txt", ios::app);
        logFile << student_id << "," << book_id << "," << borrow_date << "," << due_date << ",borrow\n";
        logFile.close();

        cout << "Mượn sách thành công! Hạn trả: " << due_date << "\n";
    }
    else
    {
        cout << "Mượn sách thất bại!\n";
    }

    cout << "\nẤn Enter để quay lại menu...";
    cin.ignore();
    cin.get();
}

void screen6_returnBook(sqlite3 *DB)
{
    int student_id;
    cout << "\n=== Trả sách ===\n";
    cout << "Nhập ID sinh viên: ";
    cin >> student_id;

    // 1️⃣ Lấy danh sách sách đang mượn
    auto list = muonsach::getBorrowingList(DB, student_id);

    if (list.empty())
    {
        cout << "Sinh viên này không mượn sách nào.\n";
        return;
    }

    cout << "\nDanh sách sách đang mượn:\n";
    cout << "BookID\tNgày mượn\tHạn trả\n";
    cout << "------------------------------------------\n";

    for (auto &m : list)
    {
        cout << m.id_book << "\t" << m.ngay_muon << "\t" << m.ngay_phai_tra << "\n";
    }

    // 2️⃣ Nhập sách cần trả
    int id_book;
    cout << "\nNhập ID sách muốn trả: ";
    cin >> id_book;

    // 3️⃣ Cập nhật muonsach
    if (!muonsach::returnBook(DB, student_id, id_book))
    {
        cout << "Lỗi khi trả sách!\n";
        return;
    }

    // 4️⃣ Giảm so_lan_dang_muon
    Book::decreaseBorrowing(DB, id_book);

    cout << "Trả sách thành công!\n";

    // 5️⃣ Kiểm tra preorder
    auto preorderList = muonsach::getPreorderList(DB, id_book);

    if (!preorderList.empty())
    {
        ofstream log("mail_log.txt", ios::app);
        for (int sid : preorderList)
        {
            log << sid << "," << id_book << ","
                << getCurrentDate() << ",reserved_ready\n";
        }
        log.close();
        cout << "Đã ghi thông báo cho sinh viên đặt trước.\n";
    }

    cout << "\nẤn Enter để quay lại menu...";
    cin.ignore();
    cin.get();
}

// Hàm helper tính ngày mai (yyyy-mm-dd)
string getTomorrowDate()
{
    time_t t = time(nullptr);
    tm *now = localtime(&t);
    now->tm_mday += 1; // ngày mai
    mktime(now);       // chuẩn hóa
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d", now);
    return string(buf);
}

void screen7_expiringBooks(sqlite3 *DB)
{
    string tomorrow = getTomorrowDate();

    // Lấy danh sách mượn sách sắp hết hạn
    vector<muonsach> borrowings = muonsach::getBorrowingList(DB, -1); // -1 = tất cả sinh viên

    cout << "\n=== Danh sách sách sắp hết hạn (còn 1 ngày) ===\n";
    cout << left << setw(5) << "ID SV"
         << setw(20) << "Tên SV"
         << setw(5) << "ID Sách"
         << setw(25) << "Tên sách"
         << setw(12) << "Ngày mượn"
         << setw(12) << "Hạn trả"
         << "\n";
    cout << "---------------------------------------------------------------------------------\n";

    bool hasRows = false;
    for (auto &m : borrowings)
    {
        if (m.trang_thai == "borrowing" && m.ngay_phai_tra == tomorrow)
        {
            hasRows = true;

            sinhvien sv = *sinhvien::login(DB, "", ""); // lấy thông tin sinh viên từ DB
            // Bạn có thể viết 1 method: sinhvien::getById(DB, m.id_sinhvien)
            const char *sql_sv = "SELECT id, ten FROM sinhvien WHERE id=?;";
            sqlite3_stmt *stmt_sv;
            if (sqlite3_prepare_v2(DB, sql_sv, -1, &stmt_sv, nullptr) == SQLITE_OK)
            {
                sqlite3_bind_int(stmt_sv, 1, m.id_sinhvien);
                if (sqlite3_step(stmt_sv) == SQLITE_ROW)
                {
                    sv.id = sqlite3_column_int(stmt_sv, 0);
                    sv.ten = reinterpret_cast<const char *>(sqlite3_column_text(stmt_sv, 1));
                }
                sqlite3_finalize(stmt_sv);
            }

            Book b = Book::getById(DB, m.id_book);

            cout << left << setw(5) << sv.id
                 << setw(20) << sv.ten
                 << setw(5) << b.id
                 << setw(25) << b.ten
                 << setw(12) << m.ngay_muon
                 << setw(12) << m.ngay_phai_tra
                 << "\n";
        }
    }

    if (!hasRows)
    {
        cout << "Không có sách nào sắp đến hạn.\n";
    }

    cout << "\nẤn Enter để quay lại menu...";
    cin.ignore();
    cin.get();
}

void mainMenuManagement(sqlite3 *DB)
{
    cout << "\n=== " << " - Main Menu Quản lý ===\n";
    int choice;
    do
    {
        cout << "\nLựa chọn:\n";
        cout << "1. Thêm sách\n";
        cout << "2. Xem toàn bộ danh sách sách\n";
        cout << "3. Mượn sách\n";
        cout << "4. Trả sách\n";
        cout << "5. Xem danh sách sách sắp hết hạn\n";
        cout << "0. Đăng xuất\n";
        cout << "Chọn: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            screen3_addBook(DB);
            break;
        case 2:
            screen4_listBooks(DB);
            break;
        case 3:
            screen5_borrowBook(DB);
            break;
        case 4:
            screen6_returnBook(DB);
            break;
        case 5:
            screen7_expiringBooks(DB);
            break;
        case 0:
            cout << "Đăng xuất...\n";
            break;
        default:
            cout << "Lựa chọn không hợp lệ.\n";
        }
    } while (choice != 0);
}
