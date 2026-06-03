// =============================================================================================
//  SCOREMANAGER.H - QUẢN LÝ ĐIỂM SỐ & KẾT QUẢ HỌC TẬP
//  – Doubly Linked List các bản ghi điểm (Score)
//  – Tính GPA học kỳ (semester) và GPA tích lũy (cumulative)
//  – Merge Sort danh sách sinh viên theo GPA tích lũy (giảm dần)
//  – Báo cáo bảng điểm 1 sinh viên / bảng điểm lớp học phần
// =============================================================================================

#pragma once
#include "Models.h"
#include "StudentManager.h"
#include "SubjectManager.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <string>

class ScoreManager {
    ScNode* head;
    ScNode* tail;
    int     count;

    // Tìm node giữa danh sách (slow pointer)
    ScNode* midNode(ScNode* h) {
        ScNode* slow = h, *fast = h ? h->next : nullptr;
        while (fast && fast->next) { slow = slow->next; fast = fast->next->next; }
        return slow;
    }

    // Trộn hai danh sách điểm đã sắp xếp giảm dần theo score10 (merge sort)
    ScNode* mergeDesc(ScNode* a, ScNode* b) {
        if (!a) return b; if (!b) return a;
        if (a->data.score10 >= b->data.score10) {
            a->next = mergeDesc(a->next, b);
            if (a->next) a->next->prev = a;
            a->prev = nullptr; return a;
        }
        b->next = mergeDesc(a, b->next);
        if (b->next) b->next->prev = b;
        b->prev = nullptr; return b;
    }

    // Sắp xếp danh sách điểm theo score10 giảm dần (merge sort)
    ScNode* sortDesc(ScNode* h) {
        if (!h || !h->next) return h;
        ScNode* mid = midNode(h);
        ScNode* second = mid->next;
        mid->next = nullptr;
        if (second) second->prev = nullptr;
        return mergeDesc(sortDesc(h), sortDesc(second));
    }

    void calcGPA(const std::string& sid,
                 const SubjectManager& sm,
                 float& outGPA4,
                 int&   outCredits) const {
    // =============================================================================================
    // Chức năng: Tính GPA tích lũy của một sinh viên dựa trên danh sách điểm.
    // Args: 
    //      sid        : MSSV của sinh viên cần tính GPA.
    //      sm         : Tham chiếu đến SubjectManager để lấy tín chỉ của môn học.
    //      outGPA4    : GPA tích lũy của sinh viên (kiểu float) được trả về qua tham số out.
    //      outCredits : Tổng số tín chỉ của sinh viên (kiểu int) được trả về qua tham số out.
    // Returns: 
    //      Không có giá trị trả về (void).
    // =============================================================================================

        float sumW  = 0.0f;
        int   sumC  = 0;
        for (ScNode* c = head; c; c = c->next) {
            if (c->data.studentId != sid) continue;
            SubNode* sub = sm.findByCode(c->data.subjectCode);
            if (!sub) continue;
            int cr = sub->data.credits;
            sumW += c->data.score4 * cr;
            sumC += cr;
        }
        outGPA4    = (sumC > 0) ? sumW / sumC : 0.0f;
        outCredits = sumC;
    }

public:
    ScoreManager() : head(nullptr), tail(nullptr), count(0) {}
    ~ScoreManager() {
        ScNode* c = head;
        while (c) { ScNode* t = c; c = c->next; delete t; }
    }

    int getCount() const { return count; }

    bool addOrUpdate(const std::string& sid, const std::string& subCode,
                     float s10,
                     const StudentManager& stm, const SubjectManager& sbm) {
    // =============================================================================================
    // Chức năng: Thêm một bản ghi điểm mới vào hệ thống. Nếu cặp (MSSV, Mã môn) đã tồn tại trong
    //            danh sách, hàm sẽ thực hiện cập nhật lại điểm số.
    // Args: 
    //      sid     : MSSV của sinh viên cần thêm/cập nhật điểm.
    //      subCode : Mã môn học tương ứng với bản ghi điểm.
    //      s10     : Điểm hệ 10 cần nhập (kiểu float).
    //      stm     : Tham chiếu đến StudentManager để kiểm tra sự tồn tại của sinh viên.
    //      sbm     : Tham chiếu đến SubjectManager để kiểm tra sự tồn tại của môn học.
    // Returns: 
    //      Trả về true nếu thêm/cập nhật thành công; trả về false nếu không tìm thấy sinh viên 
    //      hoặc môn học trong cơ sở dữ liệu.
    // =============================================================================================

        // Kiểm tra sinh viên và môn tồn tại
        if (!stm.findById(sid))         return false;
        if (!sbm.findByCode(subCode))   return false;

        float g4  = Utils::toGPA4(s10);
        std::string ltr = Utils::toLetter(g4);

        // Tìm bản ghi điểm đã có
        for (ScNode* c = head; c; c = c->next) {
            if (c->data.studentId == sid && c->data.subjectCode == subCode) {
                c->data.score10 = s10;
                c->data.score4  = g4;
                c->data.letter  = ltr;
                return true;
            }
        }

        // Chưa có điểm → thêm mới
        Score sc; sc.studentId = sid; sc.subjectCode = subCode;
        sc.score10 = s10; sc.score4 = g4; sc.letter = ltr;
        ScNode* n = new ScNode(sc);
        if (!head) { head = tail = n; }
        else { tail->next = n; n->prev = tail; tail = n; }
        count++;
        return true;
    }

    bool remove(const std::string& sid, const std::string& subCode) {
    // =============================================================================================
    // Chức năng: Tìm và xóa bản ghi điểm của một sinh viên đối với một môn học cụ thể.
    // Args: 
    //      sid     : MSSV của sinh viên cần xóa điểm.
    //      subCode : Mã môn học mà sinh viên đó muốn xóa điểm.
    // Returns: 
    //      Trả về true nếu tìm thấy và xóa thành công; trả về false nếu không tìm thấy bản ghi 
    //      điểm tương ứng để xóa.
    // =============================================================================================

        for (ScNode* c = head; c; c = c->next) {
            if (c->data.studentId == sid && c->data.subjectCode == subCode) {
                if (c->prev) c->prev->next = c->next; else head = c->next;
                if (c->next) c->next->prev = c->prev; else tail = c->prev;
                delete c; count--;
                return true;
            }
        }
        return false;
    }

    void printTranscript(const std::string& sid,
                         const StudentManager& stm,
                         const SubjectManager& sbm) const {
    // =============================================================================================
    // Chức năng: In bảng điểm chi tiết của một sinh viên cụ thể, bao gồm GPA và xếp loại.
    // Args: 
    //      sid : MSSV của sinh viên cần in bảng điểm.
    //      stm : Tham chiếu đến StudentManager để lấy thông tin sinh viên.
    //      sbm : Tham chiếu đến SubjectManager để lấy thông tin môn học.
    // Returns: 
    //      Không có giá trị trả về (void).
    // =============================================================================================

        SNode* sv = stm.findById(sid);
        if (!sv) { std::cout << "  [!] Khong tim thay sinh vien MSSV: " << sid << "\n"; return; }

        Utils::title("BANG DIEM SINH VIEN");
        std::cout << "  MSSV    : " << sv->data.id   << "\n"
                  << "  Ho ten  : " << sv->data.name << "\n"
                  << "  Lop     : " << sv->data.className << "\n"
                  << "  Nganh   : " << sv->data.major << "\n";
        Utils::line();

        std::cout << Utils::col("Ma mon", 10)
                  << Utils::col("Ten mon hoc", 30)
                  << Utils::col("TC", 5)
                  << Utils::col("Diem 10", 9)
                  << Utils::col("Diem 4", 8)
                  << Utils::col("Chu", 6) << "\n";
        Utils::line();

        int   totalC = 0;
        float sumW   = 0.0f;
        bool  found  = false;

        for (ScNode* c = head; c; c = c->next) {
            if (c->data.studentId != sid) continue;
            SubNode* sub = sbm.findByCode(c->data.subjectCode);
            std::string subName = sub ? sub->data.name : "???";
            int cr = sub ? sub->data.credits : 0;
            sumW += c->data.score4 * cr;
            totalC += cr;
            found = true;
            std::cout << Utils::col(c->data.subjectCode, 10)
                      << Utils::col(subName, 30)
                      << std::setw(4) << cr << " "
                      << std::setw(7) << std::fixed << std::setprecision(1) << c->data.score10 << "  "
                      << std::setw(5) << std::fixed << std::setprecision(1) << c->data.score4  << "  "
                      << Utils::col(c->data.letter, 6) << "\n";
        }

        if (!found) { std::cout << "  (Chua co diem nao)\n"; Utils::line(); return; }

        Utils::line();
        float gpa = (totalC > 0) ? sumW / totalC : 0.0f;
        std::cout << "  Tong tin chi tich luy : " << totalC << "\n"
                  << "  GPA tich luy (he 4)   : " << std::fixed << std::setprecision(2) << gpa << "\n"
                  << "  Xep loai hoc luc       : " << Utils::classify(gpa) << "\n";
        Utils::line();
    }

    void printClassReport(const std::string& subCode,
                          const StudentManager& stm,
                          const SubjectManager& sbm) const {
    // =============================================================================================
    // Chức năng: In bảng điểm của một lớp học phần dựa trên mã môn học, bao gồm danh sách SV
    //            và thống kê phân bổ xếp loại (A, B, C, D, F).    
    // Args: 
    //      subCode : Mã môn học cần in bảng điểm.
    //      stm     : Tham chiếu đến StudentManager để lấy tên sinh viên.
    //      sbm     : Tham chiếu đến SubjectManager để lấy thông tin môn học.
    // Returns: 
    //      Không có giá trị trả về (void).
    // =============================================================================================

        SubNode* sub = sbm.findByCode(subCode);
        if (!sub) { std::cout << "  [!] Khong tim thay mon hoc: " << subCode << "\n"; return; }

        Utils::title("BANG DIEM LOP HOC PHAN");
        std::cout << "  Ma mon  : " << sub->data.code    << "\n"
                  << "  Ten mon : " << sub->data.name    << "\n"
                  << "  Tin chi : " << sub->data.credits << "\n";
        Utils::line();

        std::cout << Utils::col("MSSV", 13)
                  << Utils::col("Ho va ten", 26)
                  << Utils::col("Lop", 12)
                  << Utils::col("Diem 10", 9)
                  << Utils::col("Diem 4", 8)
                  << Utils::col("Chu", 5) << "\n";
        Utils::line();

        // Thu thập bản ghi môn này, sắp xếp theo score10 giảm dần
        // Tạo linked list tạm để sort mà không làm hỏng list gốc
        struct TmpNode {
            std::string sid; float s10; float s4; std::string ltr;
            TmpNode* next;
        };
        TmpNode* tHead = nullptr;
        int total = 0; float sum = 0.0f;
        int cntA = 0, cntB = 0, cntC = 0, cntD = 0, cntF = 0;

        for (ScNode* c = head; c; c = c->next) {
            if (c->data.subjectCode != subCode) continue;
            TmpNode* t = new TmpNode();
            t->sid = c->data.studentId;
            t->s10 = c->data.score10;
            t->s4  = c->data.score4;
            t->ltr = c->data.letter;
            t->next = tHead; tHead = t;
            sum += c->data.score10; total++;
        }

        // Insertion sort tạm theo s10 giảm dần (danh sách nhỏ)
        for (TmpNode* i = tHead; i; i = i->next)
            for (TmpNode* j = i->next; j; j = j->next)
                if (j->s10 > i->s10) {
                    std::swap(i->sid, j->sid);
                    std::swap(i->s10, j->s10);
                    std::swap(i->s4,  j->s4);
                    std::swap(i->ltr, j->ltr);
                }

        for (TmpNode* t = tHead; t; t = t->next) {
            SNode* sv = stm.findById(t->sid);
            std::string nm = sv ? sv->data.name : "???";
            std::string cl = sv ? sv->data.className : "???";
            std::cout << Utils::col(t->sid, 13)
                      << Utils::col(nm, 26)
                      << Utils::col(cl, 12)
                      << std::setw(7) << std::fixed << std::setprecision(1) << t->s10 << "  "
                      << std::setw(5) << std::fixed << std::setprecision(1) << t->s4  << "  "
                      << Utils::col(t->ltr, 5) << "\n";
            if      (t->ltr == "A")  cntA++;
            else if (t->ltr[0]=='B') cntB++;
            else if (t->ltr[0]=='C') cntC++;
            else if (t->ltr[0]=='D') cntD++;
            else                     cntF++;
        }

        // Giải phóng tạm
        while (tHead) { TmpNode* t = tHead; tHead = t->next; delete t; }

        if (total == 0) { std::cout << "  (Chua co sinh vien nao)\n"; Utils::line(); return; }

        Utils::line();
        std::cout << "  So sinh vien : " << total << "\n"
                  << "  Diem TB lop  : " << std::fixed << std::setprecision(2) << (sum / total) << "\n";
        std::cout << "  Phan bo xep loai: "
                  << "A=" << cntA << "  B=" << cntB << "  C=" << cntC
                  << "  D=" << cntD << "  F=" << cntF << "\n";
        Utils::line();
    }

    void printRankingByGPA(const StudentManager& stm,
                           const SubjectManager& sbm) const {
    // =============================================================================================
    // Chức năng: In bảng xếp hạng toàn trường các sinh viên dựa trên GPA tích lũy, 
    //            sắp xếp theo thứ tự từ cao xuống thấp.
    // Args: 
    //      stm : Tham chiếu đến StudentManager để duyệt danh sách sinh viên.
    //      sbm : Tham chiếu đến SubjectManager để tính toán GPA.
    // Returns: 
    //      Không có giá trị trả về (void).
    // =============================================================================================

        if (stm.getCount() == 0) { std::cout << "  (Chua co sinh vien)\n"; return; }

        // Tạo mảng động
        int n = stm.getCount();
        struct Row { std::string id; std::string name; std::string cls; float gpa; int credits; };

        // Cấp phát mảng động
        Row* arr = new Row[n];
        int  idx = 0;
        for (SNode* c = stm.getHead(); c; c = c->next) {
            arr[idx].id   = c->data.id;
            arr[idx].name = c->data.name;
            arr[idx].cls  = c->data.className;
            float gpa; int cr;
            calcGPA(c->data.id, sbm, gpa, cr);
            arr[idx].gpa     = gpa;
            arr[idx].credits = cr;
            idx++;
        }

        // Merge sort mảng theo GPA giảm dần (dùng merge sort đệ quy)
        // Dùng insertion sort cho mảng
        for (int i = 1; i < n; i++) {
            Row key = arr[i];
            int j = i - 1;
            while (j >= 0 && arr[j].gpa < key.gpa) {
                arr[j + 1] = arr[j]; j--;
            }
            arr[j + 1] = key;
        }

        Utils::title("BANG XEP HANG SINH VIEN THEO GPA TICH LUY");
        std::cout << Utils::col("TT", 5)
                  << Utils::col("MSSV", 13)
                  << Utils::col("Ho va ten", 26)
                  << Utils::col("Lop", 12)
                  << Utils::col("TC", 5)
                  << Utils::col("GPA", 7)
                  << Utils::col("Xep loai", 12) << "\n";
        Utils::line();
        for (int i = 0; i < n; i++) {
            std::cout << std::setw(3) << (i + 1) << ".  "
                      << Utils::col(arr[i].id,   13)
                      << Utils::col(arr[i].name, 26)
                      << Utils::col(arr[i].cls,  12)
                      << std::setw(4) << arr[i].credits << " "
                      << std::setw(5) << std::fixed << std::setprecision(2) << arr[i].gpa << "  "
                      << Utils::classify(arr[i].gpa) << "\n";
        }
        Utils::line();
        delete[] arr;
    }

    ScNode* getHead() const { return head; }
};