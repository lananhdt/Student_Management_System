// =============================================================================================
//  SUBJECTMANAGER.H - QUẢN LÝ MÔN HỌC
//  – Doubly Linked List các đối tượng môn học (Subject)
//  – Hash Map O(1) để tra cứu môn học theo mã môn
// =============================================================================================

#pragma once
#include "Models.h"
#include "HashMap.h"
#include "Utils.h"
#include <iostream>
#include <string>

class SubjectManager {
    SubNode*       head;
    SubNode*       tail;
    int            count;
    SubjectHashMap hmap;

public:
    SubjectManager() : head(nullptr), tail(nullptr), count(0) {}
    ~SubjectManager() {
        SubNode* c = head;
        while (c) { SubNode* t = c; c = c->next; delete t; }
    }

    int     getCount() const { return count; }
    SubNode* getHead() const { return head; }

    bool add(const Subject& s) {
    // =============================================================================================
    // Chức năng: Thêm một môn học mới vào hệ thống nếu mã môn chưa tồn tại.
    // Args: 
    //      s : Đối tượng Subject chứa thông tin môn học cần thêm.
    // Returns: 
    //      Trả về true nếu thêm thành công; trả về false nếu mã môn đã tồn tại trong Hash Map.
    // =============================================================================================
        
    if (hmap.exists(s.code)) return false;
        SubNode* n = new SubNode(s);
        if (!head) { head = tail = n; }
        else { tail->next = n; n->prev = tail; tail = n; }
        hmap.put(s.code, n);
        count++;
        return true;
    }

    SubNode* findByCode(const std::string& code) const {
    // =============================================================================================
    // Chức năng: Tìm kiếm thông tin môn học dựa trên mã môn với độ phức tạp trung bình O(1).
    // Args: 
    //      code : Mã môn học cần tìm.
    // Returns: 
    //      Trả về con trỏ đến SubNode nếu tìm thấy, ngược lại trả về nullptr.
    // =============================================================================================

        return hmap.get(code);
    }

    bool findByName(const std::string& name) const {
        // =============================================================================================
        // Chức năng: Tìm kiếm học phần theo tên (hỗ trợ từ khóa).
        // Args: 
        //      name : Từ khóa tìm kiếm.
        // Returns: 
        //      Trả về true nếu tìm thấy;
        //      Trả về false nếu không.
        // =============================================================================================
            bool found = false;
            
            // Duyệt qua danh sách học phần
            for (SubNode* c = head; c; c = c->next) {
                // Kiểm tra từ khóa có nằm trong tên học phần không
                if (c->data.name.find(name) != std::string::npos) {
                    if (!found) {
                        std::cout << "\n";
                        // Định dạng cột hiển thị cho Học phần
                        std::cout << Utils::col("Mã học phần", 12)
                                  << Utils::col("Tên học phần", 33)
                                  << Utils::col("Số tín chỉ", 12) << "\n";
                        Utils::line();
                    }
                    std::cout << Utils::col(c->data.code, 12)
                              << Utils::col(c->data.name, 32)
                              << Utils::col(std::to_string(c->data.credits), 12) << "\n";
                    found = true;
                }
            }
            
            if (found) Utils::line();
            return found;
    }

    bool remove(const std::string& code) {
    // =============================================================================================
    // Chức năng: Xóa một môn học ra khỏi danh sách và cập nhật Hash Map.
    // Args: 
    //      code : Mã môn học cần xóa.
    // Returns: 
    //      Trả về true nếu xóa thành công; trả về false nếu không tìm thấy môn học.
    // =============================================================================================

        SubNode* n = hmap.get(code);
        if (!n) return false;
        n->data.isActive = false;
        return true;
    }

    bool update(const std::string& code, const Subject& newData) {
    // =============================================================================================
    // Chức năng: Cập nhật thông tin môn học (ngoại trừ mã môn).
    // Args: 
    //      code    : Mã môn học cần cập nhật.
    //      newData : Đối tượng Subject chứa thông tin mới.
    // Returns: 
    //      Trả về true nếu cập nhật thành công; 
    //      Trả về false nếu không tìm thấy môn học.
    // =============================================================================================

        SubNode* n = hmap.get(code);
        if (!n) return false;
        std::string keepCode = n->data.code;
        n->data = newData;
        n->data.code = keepCode;
        return true;
    }

    // In toàn bộ danh sách học phần đang lưu trữ trong hệ thống
    void printAll() const {
        if (!head) { std::cout << "  (Chưa có học phần nào)\n"; return; }
        std::cout << "\n";

        std::cout << Utils::col("Mã học phần", 12)
                  << Utils::col("Tên học phần", 32)
                  << Utils::col("Số tín chỉ", 12) << "\n";
        Utils::line();

        int activeCount = 0;
        for (SubNode* c = head; c; c = c->next) {
            if (!c->data.isActive) continue;
            std::cout << Utils::col(c->data.code, 12)
                      << Utils::col(c->data.name, 32)
                      << std::setw(6) << c->data.credits << "\n";
            activeCount++;
        }
        Utils::line();
        std::cout << "  Tổng số: " << count << " học phần.\n";
    }
};