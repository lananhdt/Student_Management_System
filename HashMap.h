// =============================================================================================
//  HASHMAP.H – Separate Chaining
//  – StudentHashMap: Lưu trữ ánh xạ MSSV → SNode*
//  – SubjectHashMap: Lưu trữ ánh xạ Mã môn → SubNode*
//  – Giải quyết va chạm bằng danh sách liên kết đơn (Separate Chaining)
// =============================================================================================

#pragma once
#include "Models.h"
#include <string>

class StudentHashMap {
    int CAP;          
    int size;         
    HNodeS** table;  

    int hash(const std::string& k) const {
    // =============================================================================================
    // Chức năng: Tính toán giá trị băm cho khóa
    // Args: 
    //      k : Chuỗi khóa (MSSV)
    // Returns: 
    //      Chỉ số bucket trong bảng băm
    // =============================================================================================

        long long h = 0;
        for (unsigned char c : k) h = (h * 31 + c) % CAP;
        return (int)h;
    }

    void rehash() {
        int oldCap = CAP;
        CAP = CAP * 2 + 1; // Nhân đôi kích thước và đảm bảo là số lẻ
        HNodeS** newTable = new HNodeS*[CAP](); // Cấp phát mảng mới chứa nullptr

        for (int i = 0; i < oldCap; i++) {
            HNodeS* c = table[i];
            while (c) {
                HNodeS* nextNode = c->next;
                int newIdx = hash(c->key);
                // Chèn node cũ vào đầu bucket mới
                c->next = newTable[newIdx];
                newTable[newIdx] = c;
                c = nextNode;
            }
        }
        delete[] table; // Giải phóng mảng cũ
        table = newTable; // Cập nhật mảng mới
    }

public:
    StudentHashMap() : CAP(251), size(0) { 
        table = new HNodeS*[CAP](); // Khởi tạo mảng động
    }
    ~StudentHashMap() {
        for (int i = 0; i < CAP; i++) {
            HNodeS* c = table[i];
            while (c) { HNodeS* t = c; c = c->next; delete t; }
        }
        delete[] table;
    }

    void put(const std::string& k, SNode* v) {
    // =============================================================================================
    // Chức năng: Thêm hoặc cập nhật giá trị (SNode*) ứng với khóa (MSSV)
    // Args: 
    //      k : MSSV (key); 
    //      v : Con trỏ SNode (value)
    // Returns: 
    //      Không có giá trị trả về (void).
    // =============================================================================================

        if (size >= CAP * 0.75) {rehash();}
        int i = hash(k);
        for (HNodeS* c = table[i]; c; c = c->next)
            if (c->key == k) { c->val = v; return; }
        HNodeS* n = new HNodeS(k, v);
        n->next = table[i]; table[i] = n;
        size++;
    }

    SNode* get(const std::string& k) const {
    // =============================================================================================
    // Chức năng: Tìm kiếm giá trị (SNode*) dựa trên khóa (MSSV).
    // Args: 
    //      k : MSSV cần tìm
    // Returns: 
    //      Trả về con trỏ SNode nếu tìm thấy;
    //      Trả về nullptr nếu không tồn tại.
    // =============================================================================================

        for (HNodeS* c = table[hash(k)]; c; c = c->next)
            if (c->key == k) return c->val;
        return nullptr;
    }

    void remove(const std::string& k) {
    // =============================================================================================
    // Chức năng: Xóa một cặp khóa-giá trị khỏi bảng băm.
    // Args: 
    //      k : MSSV cần xóa
    // Returns: 
    //      Không có giá trị trả về (void).
    // =============================================================================================

        int i = hash(k);
        HNodeS* c = table[i], *p = nullptr;
        while (c) {
            if (c->key == k) {
                if (p) p->next = c->next; else table[i] = c->next;
                delete c; 
                size--;
                return;
            }
            p = c; c = c->next;
        }
    }

    // Kiểm tra sự tồn tại của khóa (MSSV) trong bảng băm
    bool exists(const std::string& k) const { return get(k) != nullptr; }
};

// =============================================================================================

class SubjectHashMap {
    int CAP;
    int size;
    HNodeSub** table;

    int hash(const std::string& k) const {
    // =============================================================================================
    // Chức năng: Tính toán giá trị băm cho khóa
    // Args: 
    //      k : Chuỗi khóa (Mã học phần)
    // Returns: 
    //      Chỉ số bucket trong bảng băm
    // =============================================================================================

        long long h = 0;
        for (unsigned char c : k) h = (h * 31 + c) % CAP;
        return (int)h;
    }

    void rehash() {
        int oldCap = CAP;
        CAP = CAP * 2 + 1; 
        HNodeSub** newTable = new HNodeSub*[CAP](); 

        for (int i = 0; i < oldCap; i++) {
            HNodeSub* c = table[i];
            while (c) {
                HNodeSub* nextNode = c->next;
                int newIdx = hash(c->key);
                c->next = newTable[newIdx];
                newTable[newIdx] = c;
                c = nextNode;
            }
        }
        delete[] table; 
        table = newTable; 
    }

public:
    SubjectHashMap() : CAP(127), size(0) {table = new HNodeSub*[CAP](); }
    ~SubjectHashMap() {
        for (int i = 0; i < CAP; i++) {
            HNodeSub* c = table[i];
            while (c) { HNodeSub* t = c; c = c->next; delete t; }
        }
        delete[] table;
    }

    void put(const std::string& k, SubNode* v) {
    // =============================================================================================
    // Chức năng: Thêm hoặc cập nhật giá trị (SubNode*) ứng với khóa (Mã học phần)
    // Args: 
    //      k : Mã học phần (key); 
    //      v : Con trỏ SubNode (value)
    // Returns: 
    //      Không có giá trị trả về (void).
    // =============================================================================================

        if (size >= CAP * 0.75){ rehash(); }
        int i = hash(k);
        for (HNodeSub* c = table[i]; c; c = c->next)
            if (c->key == k) { c->val = v; return; }
        HNodeSub* n = new HNodeSub(k, v);
        n->next = table[i]; table[i] = n;
        size++;
    }

    SubNode* get(const std::string& k) const {
    // =============================================================================================
    // Chức năng: Tìm kiếm giá trị (SubNode*) dựa trên khóa (Mã học phần).
    // Args: 
    //      k : Mã học phần cần tìm
    // Returns: 
    //      Trả về con trỏ SubNode nếu tìm thấy;
    //      Trả về nullptr nếu không tồn tại.
    // =============================================================================================

        for (HNodeSub* c = table[hash(k)]; c; c = c->next)
            if (c->key == k) return c->val;
        return nullptr;
    }

    void remove(const std::string& k) {
    // =============================================================================================
    // Chức năng: Xóa một cặp khóa-giá trị khỏi bảng băm.
    // Args: 
    //      k : Mã học phần cần xóa
    // Returns: 
    //      Không có giá trị trả về (void).
    // =============================================================================================

        int i = hash(k);
        HNodeSub* c = table[i], *p = nullptr;
        while (c) {
            if (c->key == k) {
                if (p) p->next = c->next; else table[i] = c->next;
                delete c; 
                size--;
                return;
            }
            p = c; c = c->next;
        }
    }

    // Kiểm tra sự tồn tại của khóa (Mã học phần) trong bảng băm
    bool exists(const std::string& k) const { return get(k) != nullptr; }
};
