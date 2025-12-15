#include "sinhvien.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

sinhvien::sinhvien(string i, string t, string p, string e) {
    id=i; ten=t; password=p; email=e; diem_phat=0;
}

vector<sinhvien> sinhvien::getAllStudents() {
    vector<sinhvien> res;
    ifstream f("students.txt");
    string line;
    while(getline(f,line)) {
        stringstream ss(line);
        string id, ten, pw, email; int phat;
        getline(ss,id,','); getline(ss,ten,','); getline(ss,pw,','); getline(ss,email,','); ss >> phat;
        res.push_back(sinhvien(id,ten,pw,email));
        res.back().diem_phat=phat;
    }
    return res;
}

sinhvien sinhvien::getById(const string &id) {
    auto all = getAllStudents();
    for(auto &sv: all) if(sv.id==id) return sv;
    return sinhvien();
}

bool sinhvien::saveToDB() {
    auto all = getAllStudents();
    bool found=false;
    for(auto &sv: all) if(sv.id==id) { sv= *this; found=true; break; }
    if(!found) all.push_back(*this);

    ofstream f("students.txt");
    for(auto &sv: all)
        f << sv.id << "," << sv.ten << "," << sv.password << "," << sv.email << "," << sv.diem_phat << "\n";
    return true;
}
