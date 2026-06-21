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

struct SubjectScore {
    std::string subCode;
    float maxS4;
    int credits;
    bool used;
};

class ScoreManager {
    ScNode*      head;
    ScNode*      tail;
    int          count;
    bool         used;
    friend class FileManager;

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

    // Struct tạm thời để lưu điểm cao nhất của mỗi môn học khi tính CPA
    struct MaxScoreNode {
        std::string subCode;
        float maxScore4;
        int credits;
        MaxScoreNode* next;
        MaxScoreNode(std::string sc, float m, int c) : subCode(sc), maxScore4(m), credits(c), next(nullptr) {}
    };

    // Struct tạm thời để trích xuất và sắp xếp các học kỳ
    struct SemNode {
        std::string semester;
        SemNode* next;
        SemNode(std::string s) : semester(s), next(nullptr) {}
    };

    struct RankNode {
        std::string id;
        std::string name;
        std::string cls;
        float gpa;
        int credits;
        RankNode* next;
        RankNode() : next(nullptr) {}
    };

    // =============================================================================================
    // Chức năng: Tính CPA tích lũy của một sinh viên dựa trên danh sách điểm.
    //            Đã khắc phục lỗi tràn mảng (buffer overflow) bằng Singly Linked List (MaxScoreNode).
    // Args: 
    //      sid        : MSSV của sinh viên cần tính GPA.
    //      sm         : Tham chiếu đến SubjectManager để lấy tín chỉ của môn học.
    //      outCPA     : CPA tích lũy của sinh viên (kiểu float) được trả về qua tham số out.
    //      outCredits : Tổng số tín chỉ của sinh viên (kiểu int) được trả về qua tham số out.
    // Returns: 
    //      Không có giá trị trả về (void).
    // =============================================================================================

    void calcCPA(const std::string& sid, const SubjectManager& sm, float& outCPA, int& outCredits) const {
        // Khởi tạo đầu danh sách liên kết tạm thời để lưu điểm cao nhất của từng môn
        MaxScoreNode* tempHead = nullptr;

        // 1. Duyệt tất cả bản ghi điểm để lọc lấy điểm cao nhất từng môn
        for (ScNode* c = head; c; c = c->next) {
            if (c->data.studentId == sid) {
                SubNode* sub = sm.findByCode(c->data.subjectCode);
                // Bỏ qua nếu môn học không tồn tại hoặc số tín chỉ = 0
                if (!sub || sub->data.credits == 0) continue;
    
                // Tìm xem môn này đã có trong danh sách tạm chưa
                bool found = false;
                for (MaxScoreNode* t = tempHead; t; t = t->next) {
                    if (t->subCode == c->data.subjectCode) {
                        // Nếu đã học môn này và điểm mới cao hơn -> cập nhật
                        if (c->data.score4 > t->maxScore4) {
                            t->maxScore4 = c->data.score4;
                        }
                        found = true; 
                        break;
                    }
                }
                
                // Nếu chưa có trong danh sách tạm, cấp phát động và chèn vào đầu danh sách (O(1))
                if (!found) {
                    MaxScoreNode* newNode = new MaxScoreNode(c->data.subjectCode, c->data.score4, sub->data.credits);
                    newNode->next = tempHead;
                    tempHead = newNode;
                }
            }
        }
    
        // 2. Duyệt danh sách tạm để tính tổng điểm trọng số và tổng tín chỉ
        float sumW = 0.0f; 
        int sumC = 0;
        MaxScoreNode* current = tempHead;
        
        while (current) {
            sumW += current->maxScore4 * current->credits;
            sumC += current->credits;
            current = current->next;
        }
        
        // 3. Trả kết quả qua tham chiếu
        outCPA = (sumC > 0) ? (sumW / sumC) : 0.0f;
        outCredits = sumC;

        // 4. BẮT BUỘC: Giải phóng bộ nhớ động của danh sách tạm để tránh Memory Leak
        while (tempHead) {
            MaxScoreNode* tmp = tempHead;
            tempHead = tempHead->next;
            delete tmp;
        }
    }

        void calcSemesterGPA(const std::string& sid,
            const std::string& semester,
            const SubjectManager& sm,
            float& outGPA,
            int&   outCredits) const {
                float sumW  = 0.0f;
                int   sumC  = 0;

                for (ScNode* c = head; c; c = c->next) {
                // Chỉ tính những bản ghi khớp MSSV và khớp Học kỳ
                    if (c->data.studentId == sid && c->data.semester == semester) {
                        SubNode* sub = sm.findByCode(c->data.subjectCode);
                        if (!sub) continue;
                        int cr = sub->data.credits;
                        if (cr == 0) continue;
                        sumW += c->data.score4 * cr;
                        sumC += cr;
                    }
                }

                outGPA     = (sumC > 0) ? sumW / sumC : 0.0f;
                outCredits = sumC;
            }

    RankNode* mergeRank(RankNode* a, RankNode* b) const {
    if (!a) return b;
    if (!b) return a;
    if (a->gpa >= b->gpa) { // Sắp xếp giảm dần
        a->next = mergeRank(a->next, b);
        return a;
    }
    b->next = mergeRank(a, b->next);
    return b;
    }

    RankNode* splitRank(RankNode* h) const {
        RankNode* slow = h, *fast = h ? h->next : nullptr;
        while (fast && fast->next) { slow = slow->next; fast = fast->next->next; }
        return slow;
    }

    RankNode* sortRankList(RankNode* h) const {
        if (!h || !h->next) return h;
        RankNode* mid = splitRank(h);
        RankNode* sec = mid->next;
        mid->next = nullptr;
        return mergeRank(sortRankList(h), sortRankList(sec));
    }
    
public:
    bool isEmpty() const { return head == nullptr; }
    ScoreManager() : head(nullptr), tail(nullptr), count(0) {}
    ~ScoreManager() {
        ScNode* c = head;
        while (c) { ScNode* t = c; c = c->next; delete t; }
    }

    int getCount() const { return count; }

    bool addOrUpdate(const std::string& sid, const std::string& subCode,
                     const std::string& semester,
                     float cc, float tc, float gk, float ck, float s10,
                     const StudentManager& stm, const SubjectManager& sbm) {
    // =============================================================================================
    // Chức năng: Thêm một bản ghi điểm mới vào hệ thống. Nếu cặp (MSSV, Mã môn) đã tồn tại trong
    //            danh sách, hàm sẽ thực hiện cập nhật lại điểm số.
    // Args: 
    //      sid            : MSSV của sinh viên cần thêm/cập nhật điểm.
    //      subCode        : Mã môn học tương ứng với bản ghi điểm.
    //      semester       : Học kỳ của bản ghi điểm.
    //      cc, tc, gk, ck : Điểm chuyên cần, tích cực, giữa kỳ, cuối kỳ.
    //      s10            : Điểm hệ 10 cần nhập (kiểu float).
    //      stm            : Tham chiếu đến StudentManager để kiểm tra sự tồn tại của sinh viên.
    //      sbm            : Tham chiếu đến SubjectManager để kiểm tra sự tồn tại của môn học.
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
            if (c->data.studentId == sid && c->data.subjectCode == subCode && c->data.semester == semester) {
                c->data.cc       = cc;
                c->data.tc       = tc;
                c->data.gk       = gk;
                c->data.ck       = ck;
                c->data.score10  = s10;
                c->data.score4   = g4;
                c->data.letter   = ltr;
                return true;
            }
        }

        // Chưa có điểm → thêm mới
        Score sc; 
        sc.studentId = sid; 
        sc.subjectCode = subCode;
        sc.semester = semester;
        sc.cc = cc; sc.tc = tc; sc.gk = gk; sc.ck = ck;
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

    
    // =============================================================================================
    // Chức năng: In bảng điểm chi tiết của một sinh viên cụ thể, bao gồm GPA và xếp loại.
    // Args: 
    //      sid : MSSV của sinh viên cần in bảng điểm.
    //      stm : Tham chiếu đến StudentManager để lấy thông tin sinh viên.
    //      sbm : Tham chiếu đến SubjectManager để lấy thông tin môn học.
    // Returns: 
    //      Không có giá trị trả về (void).
    // =============================================================================================

    void printTranscript(const std::string& sid,
        const StudentManager& stm,
        const SubjectManager& sbm) const {
            SNode* sv = stm.findById(sid);
            if (!sv) { std::cout << "  [!] Không tìm thấy sinh viên MSSV: " << sid << "\n"; return; }

            Utils::title("BẢNG ĐIỂM CHI TIẾT SINH VIÊN");
            std::cout << "  MSSV       : " << sv->data.id   << "\n"
            << "  Họ và tên  : " << sv->data.name << "\n"
            << "  Lớp        : " << sv->data.className << "\n"
            << "  Ngành      : " << sv->data.major << "\n";
            Utils::line();

            // 1. Trích xuất các học kỳ duy nhất và sắp xếp tăng dần (Insertion Sort trên Linked List)
            SemNode* semHead = nullptr;
            for (ScNode* c = head; c; c = c->next) {
            if (c->data.studentId == sid) {
            // Kiểm tra xem học kỳ đã có trong danh sách tạm chưa
            bool exists = false;
            for (SemNode* s = semHead; s; s = s->next) {
            if (s->semester == c->data.semester) { exists = true; break; }
            }

            // Nếu chưa có, chèn vào danh sách tạm theo thứ tự alphabet (VD: 20241 -> 20242 -> 20251)
            if (!exists) {
            SemNode* n = new SemNode(c->data.semester);
            if (!semHead || semHead->semester > n->semester) {
                n->next = semHead; 
                semHead = n;
            } else {
                SemNode* p = semHead;
                while (p->next && p->next->semester < n->semester) {
                    p = p->next;
                }
                n->next = p->next; 
                p->next = n;
            }
            }
            }
            }

            if (!semHead) { std::cout << "  (Sinh viên chưa có bản ghi điểm nào)\n"; Utils::line(); return; }

            // 2. Duyệt qua từng học kỳ và in kết quả
            for (SemNode* s = semHead; s; s = s->next) {
            std::cout << "\n  >> Học kỳ: " << s->semester << "\n";
            std::cout << "  " << Utils::col("Mã học phần", 12)
                << Utils::col("Tên học phần", 35)
                << Utils::col("TC", 4)
                << Utils::col("Hệ 10", 7)
                << Utils::col("Hệ 4", 6)
                << Utils::col("Chữ", 4) << "\n";
            Utils::line('-', 75);

            float sumW = 0.0f;
            int semCredits = 0;

            for (ScNode* c = head; c; c = c->next) {
            if (c->data.studentId == sid && c->data.semester == s->semester) {
            SubNode* sub = sbm.findByCode(c->data.subjectCode);
            std::string subName = sub ? sub->data.name : "???";
            int cr = sub ? sub->data.credits : 0;
            
            sumW += c->data.score4 * cr;
            semCredits += cr;

            std::cout << "  " << Utils::col(c->data.subjectCode, 12)
                        << Utils::col(subName, 35)
                        << std::setw(2) << cr << "  "
                        << std::setw(5) << std::fixed << std::setprecision(1) << c->data.score10 << "  "
                        << std::setw(4) << std::fixed << std::setprecision(1) << c->data.score4  << "  "
                        << Utils::col(c->data.letter, 4) << "\n";
            }
            }

            // In tổng kết Học kỳ (Semester GPA)
            float semGPA = (semCredits > 0) ? sumW / semCredits : 0.0f;
            Utils::line('-', 75);
            std::cout << "  => Điểm trung bình học kỳ (GPA): " << std::fixed << std::setprecision(2) << semGPA 
                << " | Tín chỉ đạt: " << semCredits << "\n";
            }

            // 3. Giải phóng bộ nhớ danh sách học kỳ tạm
            while (semHead) { 
            SemNode* tmp = semHead; 
            semHead = semHead->next; 
            delete tmp; 
            }

            // 4. Tính toán và in Điểm trung bình tích lũy toàn khóa (CPA)
            float cpa = 0.0f; 
            int totalCredits = 0;
            calcCPA(sid, sbm, cpa, totalCredits); // Gọi hàm calcCPA đã tạo trước đó

            std::cout << "\n";
            Utils::title("TỔNG KẾT TOÀN KHÓA");
            std::cout << "  Tổng tín chỉ tích luỹ  : " << totalCredits << "\n"
            << "  CPA tích luỹ (hệ 4)    : " << std::fixed << std::setprecision(2) << cpa << "\n"
            << "  Xếp loại học lực       : " << Utils::classify(cpa) << "\n";
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
        int cA_plus = 0, cA = 0, cB_plus = 0, cB = 0;
        int cC_plus = 0, cC = 0, cD_plus = 0, cD = 0, cF = 0;

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
                    
            // 2. Phân loại chính xác 100% theo chuỗi ký tự
            if      (t->ltr == "A+") cA_plus++;
            else if (t->ltr == "A")  cA++;
            else if (t->ltr == "B+") cB_plus++;
            else if (t->ltr == "B")  cB++;
            else if (t->ltr == "C+") cC_plus++;
            else if (t->ltr == "C")  cC++;
            else if (t->ltr == "D+") cD_plus++;
            else if (t->ltr == "D")  cD++;
            else                     cF++;
        }

        // Giải phóng bộ nhớ động
        while (tHead) { TmpNode* t = tHead; tHead = t->next; delete t; }

        // 4. In tổng kết báo cáo
        Utils::line();
        std::cout << "  Số sinh viên    : " << total << "\n"
                << "  Điểm TB lớp     : " << std::fixed << std::setprecision(2) << (sum / total) << "\n";
        std::cout << "  Phân bổ xếp loại:\n"
                << "  A+: " << cA_plus << " | A: " << cA << " | B+: " << cB_plus << " | B: " << cB << "\n"
                << "  C+: " << cC_plus << " | C: " << cC << " | D+: " << cD_plus << " | D: " << cD << " | F: " << cF << "\n";
        Utils::line();
    }

    // =============================================================================================
    // Chức năng: In bảng xếp hạng toàn trường các sinh viên dựa trên GPA tích lũy, 
    //            sắp xếp theo thứ tự từ cao xuống thấp.
    // Args: 
    //      stm : Tham chiếu đến StudentManager để duyệt danh sách sinh viên.
    //      sbm : Tham chiếu đến SubjectManager để tính toán GPA.
    // Returns: 
    //      Không có giá trị trả về (void).
    // =============================================================================================

    // =============================================================================================
    // Chức năng: In bảng xếp hạng toàn trường các sinh viên dựa trên GPA tích lũy, 
    //            sắp xếp theo thứ tự từ cao xuống thấp.
    // =============================================================================================

    void printRankingByGPA(const StudentManager& stm, const SubjectManager& sbm) const {
        if (stm.getCount() == 0) { std::cout << "  (Chưa có sinh viên nào)\n"; return; }
    
        RankNode* rHead = nullptr; // Khởi tạo đầu danh sách liên kết
        
        // SỬA LỖI 1: Dùng getHead() dạng const để duyệt danh sách sinh viên một cách an toàn
        for (const SNode* c = stm.head; c; c = c->next) {
            if (c->data.status == "Đã nghỉ học") continue; // Không xếp hạng SV đã nghỉ/xóa mềm
            
            float gpa; int cr;
            calcCPA(c->data.id, sbm, gpa, cr);
            
            RankNode* r = new RankNode();
            r->id = c->data.id; r->name = c->data.name; r->cls = c->data.className;
            r->gpa = gpa; r->credits = cr;
            
            r->next = rHead; // Chèn vào đầu danh sách
            rHead = r;
        }
    
        // Gọi hàm Merge Sort thuật toán O(N log N) không tốn không gian bộ nhớ phụ
        rHead = sortRankList(rHead);
    
        // SỬA LỖI 2: Giao diện hiển thị bảng xếp hạng
        Utils::title("BẢNG XẾP HẠNG SINH VIÊN THEO GPA TÍCH LUỸ");
        std::cout << Utils::col("STT", 5)
                  << Utils::col("MSSV", 13)
                  << Utils::col("Họ và tên", 26)
                  << Utils::col("Lớp", 12)
                  << Utils::col("Số tín chỉ", 5)
                  << Utils::col("GPA", 7)
                  << Utils::col("Xếp loại", 12) << "\n";
        Utils::line();
        
        int i = 1;
        for (RankNode* curr = rHead; curr; curr = curr->next) {
            std::cout << std::setw(3) << i++ << ".  "
                      << Utils::col(curr->id,   13)
                      << Utils::col(curr->name, 26)
                      << Utils::col(curr->cls,  12)
                      << std::setw(4) << curr->credits << " "
                      << std::setw(5) << std::fixed << std::setprecision(2) << curr->gpa << "  "
                      << Utils::classify(curr->gpa) << "\n";
        }
        Utils::line();
    
        // Đừng quên giải phóng bộ nhớ danh sách RankNode để tránh rò rỉ (Memory Leak)
        while (rHead) {
            RankNode* t = rHead;
            rHead = rHead->next;
            delete t;
        }
    }

    // Thêm vào trong class ScoreManager (phần public)
    void printAll() const {
        if (isEmpty()) {
            std::cout << "  (Chưa có bản ghi điểm nào)\n";
            return;
        }
        
        std::cout << Utils::col("MSSV", 12)
                << Utils::col("Mã học phần", 15)
                << Utils::col("Điểm học phần", 10)
                << Utils::col("Điểm hệ 4", 10)
                << Utils::col("Điểm chữ", 5) << "\n";
        Utils::line();

        for (const ScNode* c = head; c; c = c->next) { // Truy cập trực tiếp 'head'
            std::cout << Utils::col(c->data.studentId,   12)
                    << Utils::col(c->data.subjectCode, 15)
                    << Utils::col(Utils::f2(c->data.score10), 10)
                    << Utils::col(Utils::f2(c->data.score4), 10)
                    << Utils::col(c->data.letter, 5) << "\n";
        }
        Utils::line();
    }
};
