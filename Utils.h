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

// =================== Tính điểm học phần hệ 10 ===================
inline float calculateFinalScore(float cc, float gk, float ck, float a, float b) {
    float diemQuaTrinh = (a * cc) + (b * gk);
    float diemHocPhan = (0.5f * diemQuaTrinh) + (0.5f * ck);
    return diemHocPhan;
}

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
    if (gpa4 >= 3.60f) return "Xuất sắc";
    if (gpa4 >= 3.20f) return "Giỏi";
    if (gpa4 >= 2.50f) return "Khá";
    if (gpa4 >= 2.00f) return "Trung bình";
    if (gpa4 >= 1.50f) return "Trung bình yếu";
    if (gpa4 >= 1.00f) return "Yếu";
    return "Kém";
}

// ========================== Hiển thị ===============================
inline void line(char c = '-', int w = 95) {
    std::cout << std::string(w, c) << "\n";
}

inline void dline(int w = 95) { line('=', w); }

inline void title(const std::string& t, int w = 95) {
    line('=', w);
    int pad = (w - (int)t.size()) / 2;
    if (pad < 0) pad = 0;
    std::cout << std::string(pad, ' ') << t << "\n";
    line('=', w);
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
        std::cout << "  [!] Chỉ nhận số nguyên. Vui lòng thử lại.\n";
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
        std::cout << "  [!] Nhập số trong [" << lo << ", " << hi << "]. Vui lòng thử lại.\n";
    }
}

// Kiểm tra điều kiện của MSSV
inline bool isValidID(const std::string& id) {
    // Kiểm tra độ dài có lớn hơn 9 không
    if (id.length() > 9) return false;
    // Kiểm tra xem tất cả các ký tự có phải là chữ số không
    for (char c : id) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// Kiểm tra điều kiện của Họ tên sinh viên
inline bool isValidName(const std::string& name) {
    if (name.empty()) return false;
    for (char c : name)
        if (isdigit((unsigned char)c) || (ispunct((unsigned char)c) && c != ' ')) 
            return false;
    return true;
}

// Kiểm tra nếu bất kỳ chuỗi nào trong danh sách bị rỗng sau khi trim
inline bool areFieldsEmpty(const std::initializer_list<std::string>& fields) {
    for (const std::string& field : fields)
        if (field.empty()) return true;
    return false;
}

inline std::string inputRequiredLine(const std::string& prompt, const std::string& fieldName) {
    std::string s;
    while (true) {
        s = inputLine(prompt);
        if (!s.empty()) return s;
        std::cout << "  [!] LỖI: " << fieldName << " không được để trống. Vui lòng nhập lại.\n";
    }
}

// ========================== UTF-8 helpers ==========================
 
// Đếm số ký tự Unicode hiển thị thực sự (code points) trong UTF-8 string.
// Mỗi ký tự tiếng Việt có dấu chiếm 2-3 bytes nhưng chỉ 1 cột hiển thị.
// std::string::size() trả về bytes → col() cũ bị lệch với tiếng Việt có dấu.
inline int utf8DisplayWidth(const std::string& s) {
    int count = 0;
    for (size_t i = 0; i < s.size(); ) {
        unsigned char c = (unsigned char)s[i];
        if      (c < 0x80) i += 1;   // ASCII 1-byte
        else if (c < 0xE0) i += 2;   // 2-byte UTF-8 (ví dụ: â, ê, ô...)
        else if (c < 0xF0) i += 3;   // 3-byte UTF-8 (ví dụ: ề, ắ, ổ...)
        else               i += 4;   // 4-byte UTF-8 (emoji, ký tự hiếm)
        count++;
    }
    return count;
}
 
// Cắt chuỗi UTF-8 về đúng displayWidth ký tự (không cắt giữa multi-byte char)
inline std::string utf8Truncate(const std::string& s, int maxDisplay) {
    int count = 0;
    size_t i = 0;
    while (i < s.size() && count < maxDisplay) {
        unsigned char c = (unsigned char)s[i];
        if      (c < 0x80) i += 1;
        else if (c < 0xE0) i += 2;
        else if (c < 0xF0) i += 3;
        else               i += 4;
        count++;
    }
    return s.substr(0, i);
}
 
// In chuỗi vào cột rộng w ký tự hiển thị, căn trái.
inline std::string col(const std::string& s, int w) {
    int dispW = utf8DisplayWidth(s);
    if (dispW >= w) {
        // Cắt tại w-1 ký tự hiển thị, thêm 1 space đệm
        return utf8Truncate(s, w - 1) + " ";
    }
    // Padding: (w - dispW) spaces — tính theo ký tự hiển thị, không phải bytes
    return s + std::string(w - dispW, ' ');
}

}