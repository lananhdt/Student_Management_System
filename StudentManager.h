// =============================================================================================
//  STUDENTMANAGER.H - QUẢN LÝ SINH VIÊN
//  – Doubly Linked List các bản ghi sinh viên (Student)
//  – Hash Map O(1) để tra cứu sinh viên theo MSSV
//  – Merge Sort tự cài (theo MSSV hoặc theo Tên)
// =============================================================================================

#pragma once
#include "Models.h"
#include "HashMap.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <string>

class StudentManager {
    SNode* head;
    SNode* tail;
    int    count;
    StudentHashMap hmap;

    // Tìm node giữa danh sách (slow pointer)
    SNode* midNode(SNode* h) {
        SNode* slow = h, *fast = h ? h->next : nullptr;
        while (fast && fast->next) { slow = slow->next; fast = fast->next->next; }
        return slow;
    }

    // Trộn hai danh sách sinh viên dựa trên hàm so sánh (cmp)
    SNode* mergeById(SNode* a, SNode* b) {
        if (!a) return b; if (!b) return a;
        if (a->data.id <= b->data.id) {
            a->next = mergeById(a->next, b);
            if (a->next) a->next->prev = a;
            a->prev = nullptr; return a;
        }
        b->next = mergeById(a, b->next);
        if (b->next) b->next->prev = b;
        b->prev = nullptr; return b;
    }

    // Sắp xếp danh sách sinh viên theo hàm so sánh (cmp)
    SNode* mergeByName(SNode* a, SNode* b) {
        if (!a) return b; if (!b) return a;
        if (a->data.name <= b->data.name) {
            a->next = mergeByName(a->next, b);
            if (a->next) a->next->prev = a;
            a->prev = nullptr; return a;
        }
        b->next = mergeByName(a, b->next);
        if (b->next) b->next->prev = b;
        b->prev = nullptr; return b;
    }

    // Merge sort GPA giảm dần — nhận mảng float* gpaArr song song
    // Vì GPA nằm ở ScoreManager, ta sort bằng cách nhận lambda-like
    // thông qua con trỏ hàm so sánh
    typedef bool (*CmpFn)(SNode*, SNode*);


    // 
    SNode* mergeWith(SNode* a, SNode* b, CmpFn cmp) {
        if (!a) return b; if (!b) return a;
        if (cmp(a, b)) {
            a->next = mergeWith(a->next, b, cmp);
            if (a->next) a->next->prev = a;
            a->prev = nullptr; return a;
        }
        b->next = mergeWith(a, b->next, cmp);
        if (b->next) b->next->prev = b;
        b->prev = nullptr; return b;
    }

    SNode* sortWith(SNode* h, CmpFn cmp) {
        if (!h || !h->next) return h;
        SNode* mid = midNode(h);
        SNode* second = mid->next;
        mid->next = nullptr;
        if (second) second->prev = nullptr;
        return mergeWith(sortWith(h, cmp), sortWith(second, cmp), cmp);
    }

    void rebuildLinks(SNode* newHead) {
        head = newHead; tail = nullptr;
        SNode* p = nullptr, *c = head;
        while (c) { c->prev = p; p = c; tail = c; c = c->next; }
    }

    static bool cmpById(SNode* a, SNode* b)   { return a->data.id   <= b->data.id; }
    static bool cmpByName(SNode* a, SNode* b)  { return a->data.name <= b->data.name; }

public:
    StudentManager() : head(nullptr), tail(nullptr), count(0) {}
    ~StudentManager() {
        SNode* c = head;
        while (c) { SNode* t = c; c = c->next; delete t; }
    }

    int    getCount() const { return count; }
    SNode* getHead()  const { return head; }

    bool add(const Student& s) {
    // =============================================================================================
    // Chức năng: Thêm một sinh viên mới vào hệ thống.
    // Args: 
    //      s : Thông tin sinh viên.
    // Returns: 
    //      Trả về true nếu thành công;
    //      Trả về false nếu MSSV đã tồn tại.
    // =============================================================================================
    
        if (hmap.exists(s.id)) return false;
        SNode* n = new SNode(s);
        if (!head) { head = tail = n; }
        else { tail->next = n; n->prev = tail; tail = n; }
        hmap.put(s.id, n);
        count++;
        return true;
    }

    SNode* findById(const std::string& id) const {
    // =============================================================================================
    // Chức năng: Tìm kiếm sinh viên theo MSSV với độ phức tạp O(1).
    // Args: 
    //      id : MSSV cần tìm.
    // Returns: 
    //      Con trỏ SNode chứa thông tin sinh viên, hoặc nullptr nếu không tìm thấy.
    // =============================================================================================

        return hmap.get(id); 
    }

    bool findByName(const std::string& kw) const {
    // =============================================================================================
    // Chức năng: Tìm kiếm sinh viên theo tên (hỗ trợ từ khóa).
    // Args: 
    //      kw : Từ khóa tìm kiếm.
    // Returns: 
    //      Trả về true nếu tìm thấy;
    //      Trả về false nếu không.
    // =============================================================================================

        bool found = false;
        for (SNode* c = head; c; c = c->next) {
            // tìm không phân biệt hoa thường: so sánh đơn giản contain
            if (c->data.name.find(kw) != std::string::npos) {
                if (!found) {
                    std::cout << "\n";
                    std::cout << Utils::col("MSSV", 12)
                              << Utils::col("Ho va ten", 24)
                              << Utils::col("Lop", 12)
                              << Utils::col("Ngay sinh", 14)
                              << Utils::col("Trang thai", 14) << "\n";
                    Utils::line();
                }
                std::cout << Utils::col(c->data.id, 12)
                          << Utils::col(c->data.name, 24)
                          << Utils::col(c->data.className, 12)
                          << Utils::col(c->data.dob, 14)
                          << Utils::col(c->data.status, 14) << "\n";
                found = true;
            }
        }
        return found;
    }

    bool remove(const std::string& id) {
    // =============================================================================================
    // Chức năng: Xóa sinh viên khỏi hệ thống theo MSSV.
    // Args: 
    //      id : MSSV cần xóa.
    // Returns: 
    //      Trả về true nếu xóa thành công.
    // =============================================================================================

        SNode* n = hmap.get(id);
        if (!n) return false;
        if (n->prev) n->prev->next = n->next; else head = n->next;
        if (n->next) n->next->prev = n->prev; else tail = n->prev;
        hmap.remove(id);
        delete n; count--;
        return true;
    }

    bool update(const std::string& id, const Student& newData) {
    // =============================================================================================
    // Chức năng: Cập nhật thông tin sinh viên (giữ nguyên MSSV).
    // Args: 
    //      id : MSSV; 
    //      newData : Thông tin mới.
    // Returns: 
    //      Trả về true nếu cập nhật thành công.
    // =============================================================================================

        SNode* n = hmap.get(id);
        if (!n) return false;
        std::string keepId = n->data.id;
        n->data = newData;
        n->data.id = keepId;
        return true;
    }

    void sortById()   { if (head) { head->prev = nullptr; rebuildLinks(sortWith(head, cmpById));   } }
    void sortByName() { if (head) { head->prev = nullptr; rebuildLinks(sortWith(head, cmpByName)); } }

    // In danh sách toàn bộ sinh viên
    void printAll() const {
        if (!head) { std::cout << "  (Chua co sinh vien nao)\n"; return; }
        std::cout << "\n";
        std::cout << Utils::col("MSSV", 13)
                  << Utils::col("Ho va ten", 24)
                  << Utils::col("Lop", 12)
                  << Utils::col("Ngay sinh", 13)
                  << Utils::col("Nganh", 18)
                  << Utils::col("Trang thai", 14) << "\n";
        Utils::line();
        for (SNode* c = head; c; c = c->next) {
            std::cout << Utils::col(c->data.id, 13)
                      << Utils::col(c->data.name, 24)
                      << Utils::col(c->data.className, 12)
                      << Utils::col(c->data.dob, 13)
                      << Utils::col(c->data.major, 18)
                      << Utils::col(c->data.status, 14) << "\n";
        }
        Utils::line();
        std::cout << "  Tong so: " << count << " sinh vien.\n";
    }
};