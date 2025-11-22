#ifndef sinhvien_h
#define sinhvien_h

#include <string>
#include <sqlite3.h>

class sinhvien
{
public:
    int id;
    std::string ten;
    std::string email;
    std::string password;

    sinhvien() {}
    sinhvien(int _id, const std::string &n, const std::string &e, const std::string &p)
        : id(_id), ten(n), email(e), password(p) {}
    sinhvien(const std::string &n, const std::string &e, const std::string &p)
        : id(0), ten(n), email(e), password(p) {}

    // Thêm sinh viên mới, dùng prepared statement
    static bool addsinhvien(sqlite3 *DB, const sinhvien &s);

    static int getBorrowingCount(sqlite3 *DB, int student_id);

    // Login: nếu đúng trả về đối tượng sinhvien, nếu sai trả về nullptr
    static sinhvien *login(sqlite3 *DB, const std::string &email, const std::string &password);
};

#endif
