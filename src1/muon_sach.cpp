


#include "muon_sach.h"
#include "dateUtils.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "sinhvien.h"

using namespace std;

muonsach::muonsach(string sid,int bid,string nm,string nt,string st) {
    id_sinhvien=sid; id_book=bid; ngay_muon=nm; ngay_phai_tra=nt; status=st;
}

bool muonsach::addToDB() {
    ofstream f("borrow.txt", ios::app);
    f << id_sinhvien << "," << id_book << "," << ngay_muon << "," << ngay_phai_tra << "," << status << ",,\n";
    return true;
}

vector<muon> muonsach::getBorrowingList(const string &sid) {
    vector<muon> res;
    ifstream f("borrow.txt");
    string line;
    while(getline(f,line)) {
        stringstream ss(line);
        muon m; char comma;
        getline(ss,m.id_sinhvien,','); ss >> m.id_book >> comma; getline(ss,m.ngay_muon,','); getline(ss,m.ngay_phai_tra,','); getline(ss,m.status,','); getline(ss,m.reserved_name1,','); getline(ss,m.reserved_svid1,',');
        if(sid=="-1" || m.id_sinhvien==sid) res.push_back(m);
    }
    return res;
}

bool muonsach::returnBook(const string &sid,int bid) {
    auto list=getBorrowingList("-1");
    bool found=false;
    for(auto &m: list) if(m.id_sinhvien==sid && m.id_book==bid && m.status=="dang muon") { m.status="tra"; found=true; break; }
    if(!found) return false;
    ofstream f("borrow.txt");
    for(auto &m: list)
        f << m.id_sinhvien << "," << m.id_book << "," << m.ngay_muon << "," << m.ngay_phai_tra << "," << m.status << "," << m.reserved_name1 << "," << m.reserved_svid1 << "\n";
    return true;
}

vector<muon> muonsach::getPreorderList(int bid) {
    auto list=getBorrowingList("-1");
    vector<muon> res;
    for(auto &m: list) if(m.id_book==bid && m.status=="reserved") res.push_back(m);
    return res;
}

void muonsach::checkAndSendRemind() {
    auto list=getBorrowingList("-1");
    string today=getCurrentDate();
    for(auto &m: list) {
        if(m.status=="dang muon") {
            int diff=getDaysBetween(today,m.ngay_phai_tra);
            if(diff==1) { // nhắc 1 ngày trước
                ofstream log("mail_log.txt",ios::app);
                log << m.id_sinhvien << "," << m.id_book << "," << today << ",remind_1day\n";
            }
            if(diff<=-5) { // trễ 5 ngày -> cộng điểm phạt
                // update diem phat
                vector<sinhvien> all=sinhvien::getAllStudents();
                for(auto &sv: all

) if(sv.id==m.id_sinhvien) { sv.diem_phat++; sv.saveToDB(); break; }
            }
        }
        if(m.status=="reserved") {
            int diff=getDaysBetween(m.ngay_muon,today);
            if(diff>3) {
                // hủy đặt trước
                m.status="canceled";
            }
        }
    }
}
