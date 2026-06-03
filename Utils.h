// =============================================================================================
//  UTILS.H - CÁC HÀM TIỆN ÍCH HỖ TRỢ
//  – Chuyển đổi điểm số (Hệ 10 sang Hệ 4, Hệ 4 sang Điểm chữ)
//  – Xếp loại học lực
//  – Các hàm hỗ trợ giao diện console và nhập liệu an toàn
// =============================================================================================

#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>

namespace Utils {

// =================== Chuyển điểm hệ 10 → hệ 4 ===================
inline float toGPA4(float s) {
    if (s >= 8.5f) return 4.0f;
    if (s >= 8.0f) return 3.5f;
    if (s >= 7.0f) return 3.0f;
    if (s >= 6.5f) return 2.5f;
    if (s >= 5.5f) return 2.0f;
    if (s >= 5.0f) return 1.5f;
    if (s >= 4.0f) return 1.0f;
    return 0.0f;
}

// =================== Chuyển điểm hệ 10 → hệ chữ ===================
inline std::string toLetter(float s) {
    if (s >= 9.5f) return "A+";
    if (s >= 8.5f) return "A";
    if (s >= 8.0f) return "B+";
    if (s >= 7.0f) return "B";
    if (s >= 6.5f) return "C+";
    if (s >= 5.5f) return "C";
    if (s >= 5.0f) return "D+";
    if (s >= 4.0f) return "D";
    return "F";
}

// =============== Xếp loại học lực theo GPA tích lũy ===============
inline std::string classify(float gpa4) {
    if (gpa4 >= 3.60f) return "Xuat sac";
    if (gpa4 >= 3.20f) return "Gioi";
    if (gpa4 >= 2.50f) return "Kha";
    if (gpa4 >= 2.00f) return "Trung binh";
    if (gpa4 >= 1.50f) return "Trung binh yeu";
    if (gpa4 >= 1.00f) return "Yeu";
    return "Kem";
}

// ========================== Hiển thị ===============================
inline void line(char c = '-', int w = 72) {
    std::cout << std::string(w, c) << "\n";
}
inline void dline(int w = 72) { line('=', w); }

inline void title(const std::string& t) {
    dline();
    int pad = (72 - (int)t.size()) / 2;
    if (pad < 0) pad = 0;
    std::cout << std::string(pad, ' ') << t << "\n";
    dline();
}

inline std::string f2(float v) {
    std::ostringstream o; o << std::fixed << std::setprecision(2) << v;
    return o.str();
}

// ========================== Input helpers ==========================

// Loại bỏ khoảng trắng/tab/xuống dòng thừa ở đầu và cuối chuỗi
inline std::string trim(const std::string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == std::string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

// Xóa bộ nhớ đệm của bàn phím để tránh lỗi nhập liệu
inline void clearCin() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Hiển thị lời nhắc và đọc một dòng văn bản từ người dùng
inline std::string inputLine(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return trim(s);
}

// Nhập số nguyên, lặp lại cho đến khi nhận dữ liệu hợp lệ
inline int inputInt(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string s; std::getline(std::cin, s);
        s = trim(s);
        if (!s.empty()) {
            bool ok = true;
            size_t start = (s[0] == '-') ? 1 : 0;
            for (size_t i = start; i < s.size(); i++)
                if (!isdigit(s[i])) { ok = false; break; }
            if (ok && s.size() > start) return std::stoi(s);
        }
        std::cout << "  [!] Chi nhan so nguyen. Thu lai.\n";
    }
}

// Nhập số thực trong khoảng [lo, hi]
inline float inputFloat(const std::string& prompt, float lo, float hi) {
    while (true) {
        std::cout << prompt;
        std::string s; std::getline(std::cin, s);
        s = trim(s);
        try {
            float v = std::stof(s);
            if (v >= lo && v <= hi) return v;
        } catch (...) {}
        std::cout << "  [!] Nhap so trong [" << lo << ", " << hi << "]. Thu lai.\n";
    }
}

// Bọc chuỗi để in ra dạng cột với độ rộng cố định, căn lề trái
inline std::string col(const std::string& s, int w) {
    if ((int)s.size() >= w) return s.substr(0, w - 1) + " ";
    return s + std::string(w - s.size(), ' ');
}

}