// =============================================================================================
//  CLASSMANAGER.H - QUẢN LÝ LỚP HỌC PHẦN
//  – Doubly Linked List cho danh sách Lớp học phần (ClassNode)
//  – Singly Linked List cho danh sách Sinh viên trong lớp (RosterNode)
// =============================================================================================

#pragma once
#include "Models.h"
#include "StudentManager.h"
#include "SubjectManager.h"
#include <iostream>
#include <string>

class ClassManager {
    ClassNode* head;
    ClassNode* tail;
    int count;
    friend class FileManager;

public:
    ClassManager() : head(nullptr), tail(nullptr), count(0) {}
    ~ClassManager() {
        ClassNode* c = head;
        while (c) {
            // Giải phóng danh sách sinh viên bên trong lớp
            RosterNode* rc = c->data.rosterHead;
            while (rc) {
                RosterNode* tempRoster = rc;
                rc = rc->next;
                delete tempRoster;
            }
            
            // Giải phóng node Lớp học phần (ClassNode)
            ClassNode* tempClass = c;
            c = c->next;
            delete tempClass;
        }
    }

    // =========================================================================================
    // Tìm kiếm lớp học phần theo mã lớp
    // =========================================================================================
    ClassNode* findByClassCode(const std::string& code) const {
        for (ClassNode* c = head; c; c = c->next) {
            if (c->data.classCode == code) return c;
        }
        return nullptr;
    }

    // =========================================================================================
    // Thêm Lớp học phần mới vào hệ thống
    // =========================================================================================
    bool addClass(const ClassSession& cs) {
        if (findByClassCode(cs.classCode)) return false; // Mã lớp đã tồn tại
        
        ClassNode* n = new ClassNode(cs);
        if (!head) { head = tail = n; }
        else { tail->next = n; n->prev = tail; tail = n; }
        count++;
        return true;
    }

    // =========================================================================================
    // Thêm sinh viên (theo MSSV) vào danh sách lớp (Roster)
    // =========================================================================================
    bool addStudentToClass(const std::string& classCode, const std::string& studentId, const StudentManager& stm) {
        ClassNode* cn = findByClassCode(classCode);
        if (!cn) return false;
        
        SNode* sv = stm.findById(studentId);
        
        // Sinh viên không tồn tại hoặc đã nghỉ học
        if (!sv || sv->data.status == "Đã nghỉ học") {
            return false; 
        }

        // Kiểm tra sinh viên đã có trong lớp chưa
        for (RosterNode* r = cn->data.rosterHead; r; r = r->next) {
            if (r->studentId == studentId) return false; // Đã có trong lớp
        }

        // Chèn sinh viên vào đầu danh sách Roster
        RosterNode* rn = new RosterNode(studentId);
        rn->next = cn->data.rosterHead;
        cn->data.rosterHead = rn;
        return true;
    }

    // =========================================================================================
    // In danh sách sinh viên của một lớp học phần
    // =========================================================================================
    void printClassRoster(const std::string& classCode, const StudentManager& stm, const SubjectManager& sbm) const {
        ClassNode* cn = findByClassCode(classCode);
        if (!cn) {
            std::cout << "  [!] Không tìm thấy Lớp học phần mã: " << classCode << "\n";
            return;
        }

        SubNode* sub = sbm.findByCode(cn->data.subjectCode);
        std::string subName = sub ? sub->data.name : "Môn học đã bị xóa/Không xác định";

        std::cout << "\n=========================================================\n";
        std::cout << "  Mã Lớp Học Phần : " << cn->data.classCode << "\n";
        std::cout << "  Môn học         : " << cn->data.subjectCode << " - " << subName << "\n";
        std::cout << "---------------------------------------------------------\n";
        std::cout << "  DANH SÁCH LỚP:\n";

        if (!cn->data.rosterHead) {
            std::cout << "  (Lớp hiện tại chưa có sinh viên nào đăng ký)\n";
            std::cout << "=========================================================\n";
            return;
        }

        int i = 1;
        for (RosterNode* r = cn->data.rosterHead; r; r = r->next) {
            SNode* sv = stm.findById(r->studentId);
            std::string svName = sv ? sv->data.name : "Sinh viên đã bị xóa/nghỉ học";
            std::cout << "  " << i++ << ". " << r->studentId << " - " << svName << "\n";
        }
        std::cout << "=========================================================\n";
    }
};
