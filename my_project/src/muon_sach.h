#ifndef muon_sach_h
#define muon_sach_h

#include <string>
#include <sqlite3.h>

class muonsach
{
public:
    int id;
    int id_sinhvien;
    int id_book;
    std::string ngay_muon;
    std::string ngay_phai_tra;
    std::string trang_thai;

    muonsach() {}
    muonsach(int sv_id, int b_id, const std::string &nm, const std::string &np, const std::string &tt)
        : id_sinhvien(sv_id), id_book(b_id), ngay_muon(nm), ngay_phai_tra(np), trang_thai(tt) {}
    static std::vector<muonsach> getBorrowingList(sqlite3 *DB, int student_id);
    static bool returnBook(sqlite3 *DB, int student_id, int book_id);
    static std::vector<int> getPreorderList(sqlite3 *DB, int book_id);

    bool addToDB(sqlite3 *DB); // Dùng DB đã mở, không tự mở/đóng
};

#endif
