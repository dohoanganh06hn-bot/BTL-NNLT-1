




#include "studentScreens.h"
#include "Book.h"
#include "muon_sach.h"
#include "dateUtils.h"
#include <iostream>
#include <fstream>

using namespace std;

void screen3_borrowBookStudent(sinhvien &sv) {
    while(true) {
        cout << "\n=== Tim & Dat sach ===\n1. Tim theo ten\n2. Tim theo tac gia\n3. Xem tat ca\n4. Dat truoc\n0. Back\nChon: ";
        int c; cin >> c; cin.ignore();
        if(c==0) break;

        vector<Book> res;
        if(c==1) {
            string k; cout << "Ten: "; getline(cin, k);
            res = Book::searchByName(k);
        } else if(c==2) {
            string k; cout << "Tac gia: "; getline(cin, k);
            res = Book::searchByAuthor(k);
        } else if(c==3) {
            res = Book::getAllBooks();
        } else if(c==4) {
            int bid; cout << "ID Sach dat truoc: "; cin >> bid;
            Book b = Book::getById(bid);
            if(b.id != 0 && b.so_lan_dang_muon >= b.number) {
                Book::preorderBook(sv.id, bid);
                ofstream log("mail_log.txt", ios::app);
                log << sv.id << "," << bid << "," << getCurrentDate() << ",reserved\n";
                cout << "Dat truoc thanh cong.\n";
            } else cout << "Sach con hoac khong ton tai.\n";
            continue;
        }
        Book::printBooks(res);
    }
}

void screen4_profileStudent(sinhvien &sv) {
    muonsach::checkAndSendRemind(); // kiểm tra nhắc nhở, cập nhật điểm phạt
    cout << "\n=== Ho so ===\nTen: " << sv.ten << "\nEmail: " << sv.email << "\nDiem phat: " << sv.diem_phat << "\n";
    cout << "Sach dang muon:\n";
    auto list = muonsach::getBorrowingList(sv.id);
    for(auto& m : list) {
        if(m.status=="dang muon") {
            Book b = Book::getById(m.id_book);
            cout << b.ten << " - Ngay muon: " << m.ngay_muon << " - Han tra: " << m.ngay_phai_tra << "\n";
        }
    }
    cin.ignore(); cin.get();
}

void mainMenuStudent(sinhvien &sv) {
    if(sv.diem_phat>3) { cout << "Ban khong duoc muon sach vi diem phat >3\n"; return; }
    int c;
    do {
        cout << "\n=== MENU HOC SINH ===\n1. Tim sach\n2. Ho so\n0. Logout\nChon: ";
        cin >> c;
        switch(c) {
            case 1: screen3_borrowBookStudent(sv); break;
            case 2: screen4_profileStudent(sv); break;
        }
    } while(c!=0);
}



