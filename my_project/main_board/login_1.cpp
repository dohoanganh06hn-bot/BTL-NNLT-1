#include <iostream>
#include <string>
#include <sqlite3.h>
#include "nhanvien.h"
#include "sinhvien.h"
#include "menuManagement.cpp"
#include "studentScreens.cpp"

using namespace std;

void mainScreen(const string &role)
{
    cout << "\n=== " << role << " đã đăng nhập thành công! ===" << endl;
}

void employeeMenu(sqlite3 *DB)
{
    int choice;
    do
    {
        cout << "\n=== NHÂN VIÊN ===\n1. Đăng ký\n2. Đăng nhập\n0. Quay lại\nChọn: ";
        cin >> choice;

        if (choice == 1)
        {
            string regPass;
            string passwordDefault = "Dohoanganh";

            cout << "Nhập mật mã đăng ký nhân viên: ";
            cin >> regPass;

            if (regPass == passwordDefault)
            {
                string tenNV, passNV;
                cout << "Nhập tên nhân viên: ";
                cin.ignore();
                getline(cin, tenNV);
                cout << "Nhập mật khẩu cho nhân viên: ";
                getline(cin, passNV);

                nhanvien emp(tenNV, passNV);
                if (nhanvien::addnhanvien(DB, emp))
                    cout << "Đăng ký nhân viên thành công!\n";
                else
                    cout << "Đăng ký thất bại! Có thể nhân viên đã tồn tại.\n";
            }
            else
            {
                cout << "Sai mật mã đăng ký!\n";
            }
        }
        else if (choice == 2)
        {
            string tenNV, passNV;
            cout << "Nhập tên nhân viên: ";
            cin.ignore();
            getline(cin, tenNV);
            cout << "Nhập mật khẩu: ";
            getline(cin, passNV);

            if (nhanvien::login(DB, tenNV, passNV))
            {
                mainScreen("Nhân viên: " + tenNV);
            }
            else
            {
                cout << "Tên hoặc mật khẩu sai!\n";
            }
        }
    } while (choice != 0);
}
void studentMenu(sqlite3 *DB)
{
    int choice;
    do
    {
        cout << "\n=== HỌC SINH ===\n1. Đăng ký\n2. Đăng nhập\n0. Quay lại\nChọn: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1)
        {
            string ten, email, pass;
            cout << "Tên: ";
            getline(cin, ten);
            cout << "Email: ";
            getline(cin, email);
            cout << "Mật khẩu: ";
            getline(cin, pass);

            sinhvien s(ten, email, pass);
            if (sinhvien::addsinhvien(DB, s))
                cout << "Đăng ký học sinh thành công!\n";
            else
                cout << "Email đã tồn tại hoặc lỗi!\n";
        }
        else if (choice == 2)
        {
            string email, pass;
            cout << "Email: ";
            getline(cin, email);
            cout << "Mật khẩu: ";
            getline(cin, pass);
            sinhvien sv;

            if (sinhvien::login(DB, email, pass))
            {
                mainMenuStudent(DB, sv);
            }
            else
            {
                cout << "Email hoặc mật khẩu sai!\n";
            }
        }
    } while (choice != 0);
}

void roleSelectionMenu(sqlite3 *DB)
{
    int choice;
    do
    {
        cout << "\n=== CHỌN VAI TRÒ ===\n1. Học sinh\n2. Nhân viên\n0. Thoát\nChọn: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            studentMenu(DB);
            break;
        case 2:
            employeeMenu(DB);
            break;
        case 0:
            cout << "Thoát chương trình.\n";
            break;
        default:
            cout << "Lựa chọn không hợp lệ!\n";
        }
    } while (choice != 0);
}

int main()
{
    sqlite3 *DB;
    if (sqlite3_open("db/library.db", &DB))
    {
        cerr << "Không thể mở DB: " << sqlite3_errmsg(DB) << endl;
        return -1;
    }

    cout << "Opened database successfully!\n";
    roleSelectionMenu(DB);

    sqlite3_close(DB);
    return 0;
}
