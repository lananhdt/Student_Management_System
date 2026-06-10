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

    // Merge sort GPA giảm dần — nhận mảng float* gpaArr song song
    typedef bool (*CmpFn)(SNode*, SNode*);

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
            // Tìm không phân biệt hoa thường
            if (c->data.name.find(kw) != std::string::npos) {
                if (!found) {
                    std::cout << "\n";
                    std::cout << Utils::col("MSSV", 11)
                              << Utils::col("Họ và tên", 25)
                              << Utils::col("Lớp", 10)
                              << Utils::col("Ngày sinh", 12)
                              << Utils::col("Ngành", 25)
                              << Utils::col("Trạng thái", 12) << "\n";
                    Utils::line();
                }
                std::cout << Utils::col(c->data.id, 11)
                          << Utils::col(c->data.name, 25)
                          << Utils::col(c->data.className, 10)
                          << Utils::col(c->data.dob, 12)
                          << Utils::col(c->data.major, 25)
                          << Utils::col(c->data.status, 12) << "\n";
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
        n->data.status = "Đã nghỉ học";
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
        if (!head) { std::cout << "  (Chưa có sinh viên nào)\n"; return; }
        std::cout << "\n";

        int wID = 11, wName = 25, wClass = 10, wDOB = 12, wMajor = 25, wStatus = 12;
        int totalWidth = wID + wName + wClass + wDOB + wMajor + wStatus;

        std::cout << Utils::col("MSSV", wID)
              << Utils::col("Họ và tên", wName)
              << Utils::col("Lớp", wClass)
              << Utils::col("Ngày sinh", wDOB)
              << Utils::col("Ngành", wMajor)
              << Utils::col("Trạng thái", wStatus) << "\n";
        Utils::line('-', totalWidth);

        int activeCount = 0;
        for (SNode* c = head; c; c = c->next) {
            if (c->data.status == "Đã nghỉ học") continue;
            std::cout << Utils::col(c->data.id, wID)
                      << Utils::col(c->data.name, wName)
                      << Utils::col(c->data.className, wClass)
                      << Utils::col(c->data.dob, wDOB)
                      << Utils::col(c->data.major, wMajor)
                      << Utils::col(c->data.status, wStatus) << "\n";
            activeCount++;
        }
        Utils::line('-', totalWidth);

        std::cout << "  Tổng số: " << count << " sinh viên.\n";
    }
};