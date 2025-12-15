#ifndef SINHVIEN_H
#define SINHVIEN_H

#include <string>
#include <vector>

class sinhvien {
public:
    std::string id;      // Thẻ học sinh
    std::string ten;
    std::string password;
    std::string email;
    int diem_phat;

    sinhvien() : diem_phat(0) {}
    sinhvien(std::string id, std::string ten, std::string password, std::string email);

    static std::vector<sinhvien> getAllStudents();
    static sinhvien getById(const std::string &id);
    bool saveToDB(); // Thêm mới hoặc cập nhật
};

#endif
