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

    struct TmpNode {
        std::string sid; float s10; float s4; std::string ltr;
        TmpNode* next;
    };

    TmpNode* mergeTmp(TmpNode* a, TmpNode* b) const {
        if (!a) return b; if (!b) return a;
        if (a->s10 >= b->s10) {
            a->next = mergeTmp(a->next, b); return a;
        }
        b->next = mergeTmp(a, b->next); return b;
    }

    TmpNode* midTmp(TmpNode* h) const {
        TmpNode* slow = h, *fast = h ? h->next : nullptr;
        while (fast && fast->next) { slow = slow->next; fast = fast->next->next; }
        return slow;
    }

    TmpNode* sortTmp(TmpNode* h) const {
        if (!h || !h->next) return h;
        TmpNode* mid = midTmp(h);
        TmpNode* sec = mid->next;
        mid->next = nullptr;
        return mergeTmp(sortTmp(h), sortTmp(sec));
    }

    struct Row { 
        std::string id; std::string name; std::string cls; float gpa; int credits; 
    };

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

    // Hàm trộn 2 nửa mảng theo GPA giảm dần
    void mergeRows(Row* arr, int l, int m, int r) const {
        int n1 = m - l + 1;
        int n2 = r - m;
        Row* L = new Row[n1];
        Row* R = new Row[n2];
        for (int i = 0; i < n1; i++) L[i] = arr[l + i];
        for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
        
        int i = 0, j = 0, k = l;
        while (i < n1 && j < n2) {
            if (L[i].gpa >= R[j].gpa) { // Sắp xếp giảm dần
                arr[k] = L[i]; i++;
            } else {
                arr[k] = R[j]; j++;
            }
            k++;
        }
        while (i < n1) { arr[k] = L[i]; i++; k++; }
        while (j < n2) { arr[k] = R[j]; j++; k++; }
        delete[] L; 
        delete[] R;
    }

    // Hàm đệ quy Merge Sort
    void mergeSortRows(Row* arr, int l, int r) const {
        if (l < r) {
            int m = l + (r - l) / 2;
            mergeSortRows(arr, l, m);
            mergeSortRows(arr, m + 1, r);
            mergeRows(arr, l, m, r);
        }
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

        // Kiểm tra sinh viên có tồn tại không
        SNode* studentNode = stm.findById(sid);
        if (!studentNode) return false;
        if (studentNode->data.status == "Đã nghỉ học") {
            std::cout << "  [!] LỖI: Sinh viên này đã nghỉ học, không thể cập nhật điểm.\n";
            return false;
        }

        // Kiểm tra học phần có tồn tại không
        if (!sbm.findByCode(subCode)) return false;

        float g4  = Utils::toGPA4(s10);
        std::string ltr = Utils::toLetter(s10);

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

    // Kiểm tra xem bản ghi điểm đã tồn tại chưa
    bool hasScoreForSubject(const std::string& subCode) const {
        for (ScNode* c = head; c; c = c->next)
            if (c->data.subjectCode == subCode) return true;
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
        if (!sv) { std::cout << "  [!] Không tìm thấy sinh viên MSSV: " << sid << "\n"; return; }

        Utils::title("BẢNG ĐIỂM SINH VIÊN");
        std::cout << "  MSSV       : " << sv->data.id   << "\n"
                  << "  Họ và tên  : " << sv->data.name << "\n"
                  << "  Lớp        : " << sv->data.className << "\n"
                  << "  Ngành      : " << sv->data.major << "\n";
        Utils::line();

        std::cout << Utils::col("Mã học phần", 10)
                  << Utils::col("Tên học phần", 30)
                  << Utils::col("Số tín chỉ", 5)
                  << Utils::col("Điểm học phần", 9)
                  << Utils::col("Điểm hệ 4", 8)
                  << Utils::col("Điểm chữ", 6) << "\n";
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

        if (!found) { std::cout << "  (Chưa có điểm nào)\n"; Utils::line(); return; }

        Utils::line();
        float gpa = (totalC > 0) ? sumW / totalC : 0.0f;
        std::cout << "  Tổng tín chỉ tích luỹ  : " << totalC << "\n"
                  << "  GPA tích luỹ (hệ 4)    : " << std::fixed << std::setprecision(2) << gpa << "\n"
                  << "  Xếp loại học lực       : " << Utils::classify(gpa) << "\n";
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
        if (!sub) { std::cout << "  [!] Không tìm thấy học phần: " << subCode << "\n"; return; }

        Utils::title("   BẢNG ĐIỂM HỌC PHẦN");
        std::cout << "  Mã học phần  : " << sub->data.code    << "\n"
                  << "  Tên học phần : " << sub->data.name    << "\n"
                  << "  Số tín chỉ   : " << sub->data.credits << "\n";
        Utils::line();

        std::cout << Utils::col("MSSV", 13)
                  << Utils::col("Họ và tên", 26)
                  << Utils::col("Lớp", 12)
                  << Utils::col("Điểm học phần", 9)
                  << Utils::col("Điểm hệ 4", 8)
                  << Utils::col("Điểm chữ", 5) << "\n";
        Utils::line();

        TmpNode* tHead = nullptr;
        int total = 0; float sum = 0.0f;

        for (ScNode* c = head; c; c = c->next) {
            if (c->data.subjectCode != subCode) continue;
            TmpNode* t = new TmpNode();
            t->sid = c->data.studentId; t->s10 = c->data.score10;
            t->s4 = c->data.score4; t->ltr = c->data.letter;
            t->next = tHead; tHead = t;
            sum += c->data.score10; total++;
        }

        if (total == 0) { std::cout << "  (Chưa có sinh viên nào)\n"; return; }

        // 2. Sắp xếp bằng Merge Sort (Thay thế hoàn toàn Insertion Sort cũ)
        tHead = sortTmp(tHead);

        // 3. In dữ liệu
        int cntA=0, cntB=0, cntC=0, cntD=0, cntF=0;
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

        if (total == 0) { std::cout << "  (Chưa có sinh viên nào)\n"; Utils::line(); return; }

        Utils::line();
        std::cout << "  Số sinh viên    : " << total << "\n"
                  << "  Điểm TB lớp     : " << std::fixed << std::setprecision(2) << (sum / total) << "\n";
        std::cout << "  Phân bổ xếp loại: "
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

        if (stm.getCount() == 0) { std::cout << "  (Chưa có sinh viên nào)\n"; return; }

        // Tạo mảng động
        int n = stm.getCount();
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

        mergeSortRows(arr, 0, n - 1);

        Utils::title("BẢNG XẾP HẠNG SINH VIÊN THEO GPA TÍCH LUỸ");
        std::cout << Utils::col("STT", 5)
                  << Utils::col("MSSV", 13)
                  << Utils::col("Họ và tên", 26)
                  << Utils::col("Lớp", 12)
                  << Utils::col("Số tín chỉ", 5)
                  << Utils::col("GPA", 7)
                  << Utils::col("Xếp loại", 12) << "\n";
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