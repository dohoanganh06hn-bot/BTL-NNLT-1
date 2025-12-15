

#include "Book.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>


using namespace std;

Book::Book() : id(0), number(0), so_lan_muon(0), so_lan_dang_muon(0) {}

Book::Book(string t,string tg,int n,string tb,int num,string the) {
    id=0; ten=t; tac_gia=tg; nam=n; tai_ban=tb; number=num; the_loai=the; so_lan_muon=0; so_lan_dang_muon=0;
}

vector<Book> Book::getAllBooks() {
    vector<Book> res;
    ifstream f("books.txt");
    string line;
    while(getline(f,line)) {
        stringstream ss(line);
        Book b; char comma;
        ss >> b.id >> comma; getline(ss,b.ten,','); getline(ss,b.tac_gia,','); ss >> b.nam >> comma; getline(ss,b.tai_ban,','); getline(ss,b.the_loai,','); ss >> b.number >> comma >> b.so_lan_muon >> comma >> b.so_lan_dang_muon;
        res.push_back(b);
    }
    return res;
}

Book Book::getById(int id) {
    auto all=getAllBooks();
    for(auto &b: all) if(b.id==id) return b;
    return Book();
}

void Book::printBooks(const vector<Book> &list) {
    cout << left
         << setw(5)  << "ID"
         << setw(22) << "Ten"
         << setw(20) << "Tac gia"
         << setw(8)  << "Nam"
         << setw(8)  << "Tai ban"
         << setw(15) << "The loai"
         << setw(10) << "So luong"
         << setw(10) << "Muon"
         << setw(12) << "Dang muon"
         << endl;

    cout << string(110, '-') << endl;

    for (const auto &b : list) {
        cout << left
             << setw(5)  << b.id
             << setw(22) << b.ten
             << setw(20) << b.tac_gia
             << setw(8)  << b.nam
             << setw(8)  << b.tai_ban
             << setw(15) << b.the_loai
             << setw(10) << b.number
             << setw(10) << b.so_lan_muon
             << setw(12) << b.so_lan_dang_muon
             << endl;
    }
}


bool Book::saveToDB() {
    auto all=getAllBooks();
    bool found=false;
    for(auto &b: all) if(b.id==id) { b=*this; found=true; break; }
    if(!found) {
        int maxId=0;
        for(auto &b: all) if(b.id>maxId) maxId=b.id;
        id=maxId+1;
        all.push_back(*this);
    }
    ofstream f("books.txt");
    for(auto &b: all)
        f << b.id << "," << b.ten << "," << b.tac_gia << "," << b.nam << "," << b.tai_ban << "," << b.the_loai << "," << b.number << "," << b.so_lan_muon << "," << b.so_lan_dang_muon << "\n";
    return true;
}

bool Book::addBook(const Book &b) { Book tmp=b; return tmp.saveToDB(); }

bool Book::updateAfterBorrow(int bid) {
    Book b=getById(bid);
    b.so_lan_muon++;
    b.so_lan_dang_muon++;
    return b.saveToDB();
}

void Book::decreaseBorrowing(int bid) {
    Book b=getById(bid);
    if(b.so_lan_dang_muon>0) b.so_lan_dang_muon--;
    b.saveToDB();
}

void Book::preorderBook(const string &sid,int bid) {
    // Logic sẽ lưu vào muon_sach, tại đây chỉ cập nhật số đang đăng ký mượn
    Book b=getById(bid);
    b.so_lan_dang_muon++;
    b.saveToDB();
}

vector<Book> Book::searchByName(const string &key) {
    vector<Book> res;
    auto all=getAllBooks();
    for(auto &b: all) if(b.ten.find(key)!=string::npos) res.push_back(b);
    return res;
}

vector<Book> Book::searchByAuthor(const string &key) {
    vector<Book> res;
    auto all=getAllBooks();
    for(auto &b: all) if(b.tac_gia.find(key)!=string::npos) res.push_back(b);
    return res;
}