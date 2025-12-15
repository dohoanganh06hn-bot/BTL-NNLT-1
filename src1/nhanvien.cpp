#include "nhanvien.h"
#include "dateUtils.h"
#include <fstream>
#include <sstream>

using namespace std;

nhanvien::nhanvien(string t, string p) { ten=t; password=p; }

vector<nhanvien> nhanvien::getAllStaff() {
    vector<nhanvien> res;
    ifstream f("staff.txt");
    string line;
    while(getline(f,line)) {
        string t,pw;
        stringstream ss(line);
        getline(ss,t,','); getline(ss,pw,',');
        res.push_back(nhanvien(t,pw));
    }
    return res;
}

nhanvien nhanvien::getByName(const string &ten) {
    auto all=getAllStaff();
    for(auto &nv: all) if(nv.ten==ten) return nv;
    return nhanvien();
}

bool nhanvien::saveToDB() {
    auto all=getAllStaff();
    bool found=false;
    for(auto &nv: all) if(nv.ten==ten) { nv=*this; found=true; break; }
    if(!found) all.push_back(*this);

    ofstream f("staff.txt");
    for(auto &nv: all) f << nv.ten << "," << nv.password << "\n";
    return true;
}

void nhanvien::logShift(const string &ten, const string &loginTime, const string &logoutTime) {
    ofstream f("staff_log.txt", ios::app);
    f << ten << "," << loginTime << "," << logoutTime << "\n";
}
