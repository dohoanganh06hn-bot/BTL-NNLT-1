#ifndef book_h
#define book_h

#include <string>
#include <vector>
#include <sqlite3.h>

class Book
{
public:
    int id;
    std::string ten;
    std::string tac_gia;
    int year;
    std::string tai_ban;
    int number;
    int so_lan_muon;
    int so_lan_dang_muon;
    std::string the_loai;

    Book() {}
    Book(std::string t, std::string a, int y, std::string tb, int n, std::string tl)
        : ten(t), tac_gia(a), year(y), tai_ban(tb), number(n), so_lan_muon(0), so_lan_dang_muon(0), the_loai(tl) {}

    // Thêm sách mới an toàn
    static bool addBook(sqlite3 *DB, const Book &b);
    // Trả về sách theo ID
    static Book getById(sqlite3 *DB, int book_id);

    // Cập nhật sau khi mượn: tăng so_lan_muon và so_lan_dang_muon
    static bool updateAfterBorrow(sqlite3 *DB, int book_id);
    static bool decreaseBorrowing(sqlite3 *DB, int book_id);

    // Truy vấn sách
    static std::vector<Book> searchByName(sqlite3 *DB, const std::string &keyword);
    static std::vector<Book> searchByAuthor(sqlite3 *DB, const std::string &keyword);
    static std::vector<Book> searchByCategory(sqlite3 *DB, const std::vector<int> &categories);
    static std::vector<Book> getAllBooks(sqlite3 *DB);

    // Đặt trước sách (preorder)
    static bool preorderBook(sqlite3 *DB, int student_id, int book_id);
};

#endif
