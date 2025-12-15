#include "studentScreens.h"
#include "menuManagement.h"
#include "sinhvien.h"
#include "nhanvien.h"
#include "dateUtils.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
    while (true) {
        cout << "\n=== HE THONG THU VIEN ===\n";
        cout << "1. Sinh vien\n2. Nhan vien\n0. Thoat\nChon: ";
        int role; cin >> role; cin.ignore();

        if (role == 0) break;

        if (role == 1) { // Sinh vien
            cout << "1. Dang ky\n2. Dang nhap\n0. Back\nChon: ";
            int choice; cin >> choice; cin.ignore();

            if (choice == 0) continue;

            if (choice == 1) { // Dang ky sinh vien
                string id, ten, email, pw;
                cout << "Nhap ID sinh vien: "; getline(cin, id);
                cout << "Nhap ten: "; getline(cin, ten);
                cout << "Nhap email: "; getline(cin, email);
                cout << "Nhap mat khau: "; getline(cin, pw);

                if (!sinhvien::getById(id).id.empty()) {
                    cout << "ID da ton tai!\n";
                    continue;
                }

                sinhvien sv(id, ten, pw, email);
                if (sv.saveToDB()) cout << "Dang ky thanh cong!\n";
                else cout << "Co loi khi luu du lieu!\n";

            } else if (choice == 2) { // Dang nhap sinh vien
                string id, pw;
                cout << "Nhap ID: "; getline(cin, id);
                cout << "Nhap mat khau: "; getline(cin, pw);

                sinhvien sv = sinhvien::getById(id);
                if (sv.id.empty() || sv.password != pw) {
                    cout << "Dang nhap khong thanh cong!\n";
                } else {
                    cout << "Dang nhap thanh cong!\n";
                    mainMenuStudent(sv);
                }
            }

        } else if (role == 2) { // Nhan vien
            cout << "1. Dang ky\n2. Dang nhap\n0. Back\nChon: ";
            int choice; cin >> choice; cin.ignore();

            if (choice == 0) continue;

            if (choice == 1) { // Dang ky nhan vien
                string name, pw;
                cout << "Nhap ten nhan vien: "; getline(cin, name);
                cout << "Nhap mat khau: "; getline(cin, pw);

                if (!nhanvien::getByName(name).ten.empty()) {
                    cout << "Ten da ton tai!\n";
                    continue;
                }

                nhanvien nv(name, pw);
                if (nv.saveToDB()) cout << "Dang ky nhan vien thanh cong!\n";
                else cout << "Co loi khi luu du lieu!\n";

            } else if (choice == 2) { // Dang nhap nhan vien
                string name, pw;
                cout << "Nhap ten nhan vien: "; getline(cin, name);
                cout << "Nhap mat khau: "; getline(cin, pw);

                nhanvien nv = nhanvien::getByName(name);
                if (nv.ten.empty() || nv.password != pw) {
                    cout << "Dang nhap khong thanh cong!\n";
                } else {
                    string loginTime = getCurrentDate();
                    nhanvien::logShift(name, loginTime, ""); // bat dau ca
                    cout << "Dang nhap thanh cong!\n";
                    mainMenuManagement();
                    string logoutTime = getCurrentDate();
                    nhanvien::logShift(name, loginTime, logoutTime); // ket thuc ca
                }
            }

        } else {
            cout << "Lua chon khong hop le!\n";
        }
    }

    cout << "Thoat chuong trinh.\n";
    return 0;
}
