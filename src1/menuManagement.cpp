

#include "menuManagement.h"
#include "Book.h"
#include "sinhvien.h"
#include "muon_sach.h"
#include "nhanvien.h"
#include "dateUtils.h"
#include <iostream>
#include <fstream>

using namespace std;

void screen3_addBook() {
    cin.ignore();
    string ten, tac, tai, the;
    int year, num;
    cout << "\n=== Them sach ===\nTen: "; getline(cin, ten);
    cout << "Tac gia: "; getline(cin, tac);
    cout << "Nam: "; cin >> year; cin.ignore();
    cout << "Tai ban: "; getline(cin, tai);
    cout << "So luong: "; cin >> num; cin.ignore();
    cout << "The loai: "; getline(cin, the);

    Book b(ten, tac, year, tai, num, the);
    if(Book::addBook(b)) cout << "Them sach thanh cong!\n";
    else cout << "Loi!\n";
}

void screen4_listBooks() {
    auto books = Book::getAllBooks();
    Book::printBooks(books);
    cout << "An Enter de quay lai..."; cin.ignore(); cin.get();
}

void screen5_borrowBook() {
    int sid, bid;
    cout << "\n=== Muon sach ===\nID SV: "; cin >> sid;
    cout << "ID Sach: "; cin >> bid;

    Book b = Book::getById(bid);
    if(b.id == 0) { cout << "Sach khong ton tai!\n"; return; }

    if(b.so_lan_dang_muon >= b.number) {
        cout << "Sach da het. Dat truoc? (y/n): ";
        char c; cin >> c;
        if(c == 'y') {
            Book::preorderBook(to_string(sid), bid);
            ofstream log("mail_log.txt", ios::app);
            log << sid << "," << bid << "," << getCurrentDate() << ",reserved\n";
            cout << "Da dat truoc.\n";
        }
        return;
    }

    string borrow = getCurrentDate();
    string due = getDueDate(1);
    muonsach m(to_string(sid), bid, borrow, due, "dang muon");
    
    if(m.addToDB() && Book::updateAfterBorrow(bid)) {
        ofstream log("mail_log.txt", ios::app);
        log << sid << "," << bid << "," << borrow << "," << due << ",borrow\n";
        cout << "Muon thanh cong! Han tra: " << due << "\n";
    } else cout << "Loi muon sach!\n";
}

void screen6_returnBook() {
    int sid; cout << "\n=== Tra sach ===\nID SV: "; cin >> sid;
    auto list = muonsach::getBorrowingList(to_string(sid));
    if(list.empty()) { cout << "Khong co sach dang muon.\n"; return; }

    for(auto& m : list) cout << m.id_book << "\t" << m.ngay_muon << "\t" << m.ngay_phai_tra << "\n";
    
    int bid; cout << "Nhap ID sach tra: "; cin >> bid;
    if(muonsach::returnBook(to_string(sid), bid)) {
        Book::decreaseBorrowing(bid);
        cout << "Tra thanh cong.\n";
        // Check preorder
        auto pre = muonsach::getPreorderList(bid);
        if(!pre.empty()) {
            ofstream log("mail_log.txt", ios::app);
            for(auto &p : pre) log << p.id_sinhvien << "," << bid << "," << getCurrentDate() << ",reserved_ready\n";
            cout << "Da thong bao cho SV dat truoc.\n";
        }
    } else cout << "Loi tra sach!\n";
}

void screen7_expiringBooks() {
    string tom = getTomorrowDate();
    auto list = muonsach::getBorrowingList("-1");
    cout << "\n=== Sach sap het han ===\n";
    for(auto& m : list) {
        if(m.ngay_phai_tra == tom && m.status=="dang muon") {
            sinhvien sv = sinhvien::getById(m.id_sinhvien);
            Book b = Book::getById(m.id_book);
            cout << sv.ten << " - " << b.ten << " - " << m.ngay_phai_tra << "\n";
        }
    }
    cin.ignore();
}
void mainMenuManagement() {
    int c;
    do {
        cout << "\n=== MENU QUAN LY ===\n1. Them sach\n2. Xem sach\n3. Muon sach\n4. Tra sach\n5. Sap het han\n0. Logout\nChon: ";
        cin >> c;
        switch(c) {
            case 1: screen3_addBook(); break;
            case 2: screen4_listBooks(); break;
            case 3: screen5_borrowBook(); break;
            case 4: screen6_returnBook(); break;
            case 5: screen7_expiringBooks(); break;
        }
    } while(c != 0);
}
