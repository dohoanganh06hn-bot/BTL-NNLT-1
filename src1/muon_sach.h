
#ifndef MUON_SACH_H
#define MUON_SACH_H

#include <string>
#include <vector>

struct muon {
    std::string id_sinhvien;
    int id_book;
    std::string ngay_muon;
    std::string ngay_phai_tra;
    std::string status; // "dang muon","tra","reserved"
    std::string reserved_svid1; // nếu có đặt trước
    std::string reserved_name1;
};

class muonsach {
public:
    std::string id_sinhvien;
    int id_book;
    std::string ngay_muon;
    std::string ngay_phai_tra;
    std::string status;

    muonsach() {}
    muonsach(std::string sid,int bid,std::string nm,std::string nt,std::string st);

    bool addToDB();
    static std::vector<muon> getBorrowingList(const std::string &sid); // sid=-1 lấy tất cả
    static bool returnBook(const std::string &sid,int bid);
    static std::vector<muon> getPreorderList(int bid);
    static void checkAndSendRemind(); // gửi nhắc nhở + cập nhật điểm phạt
};

#endif