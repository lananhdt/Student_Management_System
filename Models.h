// =============================================================================================
//  MODELS.H  –  Toàn bộ cấu trúc dữ liệu thô
//  Định nghĩa các cấu trúc dữ liệu cơ bản cho hệ thống.
// =============================================================================================

#pragma once
#include <string>

// ======================================Thực thể nghiệp vụ =====================================
struct Student {
    std::string id;         // MSSV
    std::string name;       // Họ và tên
    std::string className;  // Lớp học
    std::string dob;        // Ngày sinh DD/MM/YYYY
    std::string major;      // Ngành học
    std::string email;      // Địa chỉ email liên lạc
    std::string status;     // Đang học / Bảo lưu / Cảnh cáo / Đã tốt nghiệp
};

struct Subject {
    std::string code;       // Mã học phần
    std::string name;       // Tên học phần
    int         credits;    // Số tín chỉ
    bool        isActive;   // true: hoạt động, false: đã xóa

    Subject() : isActive(true), credits(0) {}
};

struct Score {
    std::string studentId;   // MSSV
    std::string subjectCode; // Mã học phần
    float       score10;     // Điểm hệ 10
    float       score4;      // Điểm hệ 4
    std::string letter;      // A, B+, ...
};

struct RosterNode {
    std::string studentId;
    RosterNode* next;
    RosterNode(const std::string& id) : studentId(id), next(nullptr) {}
};

struct ClassSession {
    std::string classCode;  // Mã lớp học phần (VD: 158241)
    std::string subjectCode;// Mã môn học (VD: MI3310)
    RosterNode* rosterHead; // Danh sách liên kết đơn chứa MSSV
    
    ClassSession() : rosterHead(nullptr) {}
};

// =================================== Node Doubly Linked List ===================================
struct SNode {   // Student node
    Student data;
    SNode*  prev;
    SNode*  next;
    SNode(const Student& s) : data(s), prev(nullptr), next(nullptr) {}
};

struct SubNode { // Subject node
    Subject  data;
    SubNode* prev;
    SubNode* next;
    SubNode(const Subject& s) : data(s), prev(nullptr), next(nullptr) {}
};

struct ScNode {  // Score node
    Score   data;
    ScNode* prev;
    ScNode* next;
    ScNode(const Score& s) : data(s), prev(nullptr), next(nullptr) {}
};

struct ClassNode {
    ClassSession data;
    ClassNode* prev;
    ClassNode* next;
    ClassNode(const ClassSession& c) : data(c), prev(nullptr), next(nullptr) {}
};

// ================================== Hash node (chaining) ==================================
struct HNodeS {          // hash → SNode
    std::string key;
    SNode*      val;
    HNodeS*     next;
    HNodeS(const std::string& k, SNode* v) : key(k), val(v), next(nullptr) {}
};

struct HNodeSub {        // hash → SubNode
    std::string key;
    SubNode*    val;
    HNodeSub*   next;
    HNodeSub(const std::string& k, SubNode* v) : key(k), val(v), next(nullptr) {}
};