#ifndef NHANVIEN_H
#define NHANVIEN_H

#include <string>
#include <vector>

class nhanvien {
public:
    std::string ten;
    std::string password;

    nhanvien() {}
    nhanvien(std::string ten, std::string pw);

    static std::vector<nhanvien> getAllStaff();
    static nhanvien getByName(const std::string &ten);

    bool saveToDB();
    static void logShift(const std::string &ten, const std::string &loginTime, const std::string &logoutTime="");
};

#endif
