#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <vector>

class Book {
public:
    int id;
    std::string ten;
    std::string tac_gia;
    int nam;
    std::string tai_ban;
    std::string the_loai; // giáo trình/tham khảo
    int number;
    int so_lan_muon;
    int so_lan_dang_muon;

    Book();
    Book(std::string ten,std::string tac,int nam,std::string tai,int num,std::string the);

    static std::vector<Book> getAllBooks();
    static Book getById(int id);
    static void printBooks(const std::vector<Book> &list);

    bool saveToDB(); // thêm hoặc cập nhật
    static bool addBook(const Book &b);
    static bool updateAfterBorrow(int id);
    static void decreaseBorrowing(int id);
    static void preorderBook(const std::string &sid,int bid);
    static std::vector<Book> searchByName(const std::string &key);
    static std::vector<Book> searchByAuthor(const std::string &key);
};

#endif