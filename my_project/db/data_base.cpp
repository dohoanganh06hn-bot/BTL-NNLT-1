#include <sqlite3.h>
#include <iostream>

bool execSQL(sqlite3 *DB, const std::string &sql)
{
    char *errMsg;
    if (sqlite3_exec(DB, sql.c_str(), nullptr, 0, &errMsg) != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

int main()
{
    sqlite3 *DB;
    if (sqlite3_open("db/library.db", &DB))
    {
        std::cerr << "Can't open DB: " << sqlite3_errmsg(DB) << std::endl;
        return -1;
    }

    std::cout << "Opened database successfully!" << std::endl;

    // Tạo bảng nhanvien
    execSQL(DB, "CREATE TABLE IF NOT EXISTS nhanvien("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "ten TEXT UNIQUE,"
                "password TEXT);");

    // Tạo bảng sinhvien
    execSQL(DB, "CREATE TABLE IF NOT EXISTS sinhvien("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "ten TEXT,"
                "email TEXT UNIQUE,"
                "password TEXT);");

    // Tạo bảng book
    execSQL(DB, "CREATE TABLE IF NOT EXISTS book("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "ten TEXT,"
                "tac_gia TEXT,"
                "year INTEGER,"
                "tai_ban TEXT,"
                "number INTEGER,"
                "so_lan_muon INTEGER DEFAULT 0,"
                "so_lan_dang_muon INTEGER DEFAULT 0,"
                "the_loai TEXT);");

    // Tạo bảng muonsach với FOREIGN KEY
    execSQL(DB, "CREATE TABLE IF NOT EXISTS muonsach("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "id_sinhvien INTEGER,"
                "id_book INTEGER,"
                "ngay_muon TEXT,"
                "ngay_phai_tra TEXT,"
                "trang_thai TEXT,"
                "FOREIGN KEY(id_sinhvien) REFERENCES sinhvien(id),"
                "FOREIGN KEY(id_book) REFERENCES book(id));");

    // Tạo bảng preorder với FOREIGN KEY
    execSQL(DB, "CREATE TABLE IF NOT EXISTS preorder("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "id_sinhvien INTEGER,"
                "id_book INTEGER,"
                "request_date TEXT,"
                "FOREIGN KEY(id_sinhvien) REFERENCES sinhvien(id),"
                "FOREIGN KEY(id_book) REFERENCES book(id));");

    sqlite3_close(DB);
    return 0;
}
