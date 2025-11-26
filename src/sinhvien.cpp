#include "sinhvien.h"
#include <iostream>

bool sinhvien::addsinhvien(sqlite3 *DB, const sinhvien &s)
{
    std::string sql =
        "INSERT INTO sinhvien(ten, email, password) VALUES('" +
        s.ten + "', '" + s.email + "', '" + s.password + "');";

    char *errMsg = nullptr;
    if (sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        std::cerr << "Insert failed: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

static int loginCallback(void *data, int argc, char **argv, char **colName)
{
    if (argc == 4)
    {
        sinhvien **sv = (sinhvien **)data;
        *sv = new sinhvien(
            std::stoi(argv[0]),
            argv[1] ? argv[1] : "",
            argv[2] ? argv[2] : "",
            argv[3] ? argv[3] : "");
    }
    return 0;
}

sinhvien *sinhvien::login(sqlite3 *DB, const std::string &email, const std::string &password)
{
    std::string sql =
        "SELECT id, ten, email, password FROM sinhvien "
        "WHERE email='" +
        email + "' AND password='" + password + "';";

    sinhvien *sv = nullptr;
    char *errMsg = nullptr;

    sqlite3_exec(DB, sql.c_str(), loginCallback, &sv, &errMsg);

    if (errMsg)
    {
        std::cerr << "Login error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    return sv; // nullptr = login thất bại
}

static int countCallback(void *data, int argc, char **argv, char **col)
{
    int *count = (int *)data;
    *count = std::stoi(argv[0]);
    return 0;
}

int sinhvien::getBorrowingCount(sqlite3 *DB, int id_sinhvien)
{
    std::string sql =
        "SELECT COUNT(*) FROM muonsach WHERE id_sinhvien=" +
        std::to_string(id_sinhvien) +
        " AND trang_thai='borrowing';";

    int count = 0;
    char *errMsg = nullptr;

    sqlite3_exec(DB, sql.c_str(), countCallback, &count, &errMsg);

    if (errMsg)
    {
        std::cerr << "Count error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    return count;
}
