// =============================================================================================
//  MENU.H – HỆ THỐNG ĐIỀU KHIỂN GIAO DIỆN
//  – Điều hướng người dùng qua 4 module chính
//  – Xử lý logic nhập liệu và gọi các hàm nghiệp vụ của các Manager
// =============================================================================================

#pragma once
#include "StudentManager.h"
#include "SubjectManager.h"
#include "ScoreManager.h"
#include "FileManager.h"
#include "Utils.h"
#include <iostream>
#include <string>

class Menu {
    StudentManager& stm;
    SubjectManager& sbm;
    ScoreManager&   scm;

    // Hiển thị giao diện menu chính lên console
    void showMain() const {
        Utils::dline();
        std::cout << "         HE THONG QUAN LY SINH VIEN & DIEM SO\n";
        Utils::dline();
        std::cout << "  [1]  Quan ly Sinh vien\n"
                  << "  [2]  Quan ly Mon hoc\n"
                  << "  [3]  Quan ly Diem so\n"
                  << "  [4]  Bao cao & Tim kiem\n"
                  << "  [0]  Thoat chuong trinh\n";
        Utils::dline();
    }

    // ============================================================
    //  MENU 1: SINH VIÊN
    // ============================================================
    void menuStudent() {
        while (true) {
            Utils::title("QUAN LY SINH VIEN");
            std::cout << "  [1] Xem danh sach sinh vien\n"
                      << "  [2] Them sinh vien moi\n"
                      << "  [3] Sua thong tin sinh vien\n"
                      << "  [4] Xoa sinh vien\n"
                      << "  [5] Tim kiem theo MSSV\n"
                      << "  [6] Tim kiem theo ho ten\n"
                      << "  [7] Sap xep theo MSSV\n"
                      << "  [8] Sap xep theo ho ten\n"
                      << "  [0] Quay lai\n";
            Utils::line();
            int ch = Utils::inputInt("  > Chon: ");
            std::cout << "\n";

            if (ch == 0) break;

            else if (ch == 1) {
                Utils::title("DANH SACH SINH VIEN");
                stm.printAll();
            }

            else if (ch == 2) {
                Utils::title("THEM SINH VIEN MOI");
                Student s;
                s.id        = Utils::inputLine("  MSSV         : ");
                if (s.id.empty()) { std::cout << "  [!] MSSV khong duoc trong.\n"; continue; }
                if (stm.findById(s.id)) { std::cout << "  [!] MSSV da ton tai.\n"; continue; }
                s.name      = Utils::inputLine("  Ho va ten    : ");
                s.className = Utils::inputLine("  Lop          : ");
                s.dob       = Utils::inputLine("  Ngay sinh    : ");
                s.major     = Utils::inputLine("  Nganh hoc    : ");
                s.email     = Utils::inputLine("  Email        : ");
                std::cout << "  Trang thai (1-Dang hoc / 2-Bao luu / 3-Canh bao / 4-Tot nghiep): ";
                int st = Utils::inputInt("");
                if      (st == 1) s.status = "Dang hoc";
                else if (st == 2) s.status = "Bao luu";
                else if (st == 3) s.status = "Canh bao";
                else              s.status = "Tot nghiep";
                stm.add(s);
                FileManager::saveStudents("students.txt", stm);
                std::cout << "  [OK] Da them sinh vien " << s.id << " thanh cong.\n";
            }

            else if (ch == 3) {
                Utils::title("SUA THONG TIN SINH VIEN");
                std::string id = Utils::inputLine("  Nhap MSSV can sua: ");
                SNode* node = stm.findById(id);
                if (!node) { std::cout << "  [!] Khong tim thay MSSV: " << id << "\n"; continue; }
                std::cout << "  (Bo trong de giu nguyen gia tri cu)\n";
                Student s = node->data;
                std::string tmp;
                tmp = Utils::inputLine("  Ho va ten    [" + s.name      + "]: "); if (!tmp.empty()) s.name      = tmp;
                tmp = Utils::inputLine("  Lop          [" + s.className + "]: "); if (!tmp.empty()) s.className = tmp;
                tmp = Utils::inputLine("  Ngay sinh    [" + s.dob       + "]: "); if (!tmp.empty()) s.dob       = tmp;
                tmp = Utils::inputLine("  Nganh hoc    [" + s.major     + "]: "); if (!tmp.empty()) s.major     = tmp;
                tmp = Utils::inputLine("  Email        [" + s.email     + "]: "); if (!tmp.empty()) s.email     = tmp;
                tmp = Utils::inputLine("  Trang thai   [" + s.status    + "]: "); if (!tmp.empty()) s.status    = tmp;
                stm.update(id, s);
                FileManager::saveStudents("students.txt", stm);
                std::cout << "  [OK] Da cap nhat sinh vien " << id << ".\n";
            }

            else if (ch == 4) {
                Utils::title("XOA SINH VIEN");
                std::string id = Utils::inputLine("  Nhap MSSV can xoa: ");
                if (!stm.findById(id)) { std::cout << "  [!] Khong tim thay MSSV: " << id << "\n"; continue; }
                std::cout << "  Xac nhan xoa " << id << "? (y/n): ";
                std::string cf; std::getline(std::cin, cf);
                if (Utils::trim(cf) == "y" || Utils::trim(cf) == "Y") {
                    stm.remove(id);
                    FileManager::saveStudents("students.txt", stm);
                    std::cout << "  [OK] Da xoa sinh vien " << id << ".\n";
                } else {
                    std::cout << "  Da huy.\n";
                }
            }

            else if (ch == 5) {
                std::string id = Utils::inputLine("  Nhap MSSV: ");
                SNode* n = stm.findById(id);
                if (!n) { std::cout << "  [!] Khong tim thay.\n"; continue; }
                Utils::line();
                std::cout << "  MSSV    : " << n->data.id        << "\n"
                          << "  Ho ten  : " << n->data.name      << "\n"
                          << "  Lop     : " << n->data.className << "\n"
                          << "  Ngay SN : " << n->data.dob       << "\n"
                          << "  Nganh   : " << n->data.major     << "\n"
                          << "  Email   : " << n->data.email     << "\n"
                          << "  Trang thai: " << n->data.status  << "\n";
                Utils::line();
            }

            else if (ch == 6) {
                std::string kw = Utils::inputLine("  Nhap tu khoa ho ten: ");
                if (!stm.findByName(kw))
                    std::cout << "  [!] Khong tim thay sinh vien nao.\n";
            }

            else if (ch == 7) {
                stm.sortById();
                FileManager::saveStudents("students.txt", stm);
                std::cout << "  [OK] Da sap xep theo MSSV tang dan.\n";
                stm.printAll();
            }

            else if (ch == 8) {
                stm.sortByName();
                FileManager::saveStudents("students.txt", stm);
                std::cout << "  [OK] Da sap xep theo ho ten tang dan.\n";
                stm.printAll();
            }

            else {
                std::cout << "  [!] Lua chon khong hop le.\n";
            }
            std::cout << "\n";
        }
    }

    // ============================================================
    //  MENU 2: MÔN HỌC
    // ============================================================
    void menuSubject() {
        while (true) {
            Utils::title("QUAN LY MON HOC");
            std::cout << "  [1] Xem danh sach mon hoc\n"
                      << "  [2] Them mon hoc moi\n"
                      << "  [3] Sua thong tin mon hoc\n"
                      << "  [4] Xoa mon hoc\n"
                      << "  [5] Tim kiem theo ma mon\n"
                      << "  [0] Quay lai\n";
            Utils::line();
            int ch = Utils::inputInt("  > Chon: ");
            std::cout << "\n";
            if (ch == 0) break;

            else if (ch == 1) {
                Utils::title("DANH SACH MON HOC");
                sbm.printAll();
            }

            else if (ch == 2) {
                Utils::title("THEM MON HOC MOI");
                Subject s;
                s.code = Utils::inputLine("  Ma mon hoc  : ");
                if (s.code.empty()) { std::cout << "  [!] Ma mon khong duoc trong.\n"; continue; }
                if (sbm.findByCode(s.code)) { std::cout << "  [!] Ma mon da ton tai.\n"; continue; }
                s.name    = Utils::inputLine("  Ten mon hoc : ");
                s.credits = Utils::inputInt ("  So tin chi  : ");
                sbm.add(s);
                FileManager::saveSubjects("subjects.txt", sbm);
                std::cout << "  [OK] Da them mon hoc " << s.code << " thanh cong.\n";
            }

            else if (ch == 3) {
                Utils::title("SUA MON HOC");
                std::string code = Utils::inputLine("  Nhap ma mon can sua: ");
                SubNode* node = sbm.findByCode(code);
                if (!node) { std::cout << "  [!] Khong tim thay ma mon: " << code << "\n"; continue; }
                Subject s = node->data;
                std::string tmp;
                tmp = Utils::inputLine("  Ten mon     [" + s.name + "]: ");
                if (!tmp.empty()) s.name = tmp;
                std::cout << "  So tin chi  [" << s.credits << "]: ";
                std::string tc; std::getline(std::cin, tc); tc = Utils::trim(tc);
                if (!tc.empty()) try { s.credits = std::stoi(tc); } catch (...) {}
                sbm.update(code, s);
                FileManager::saveSubjects("subjects.txt", sbm);
                std::cout << "  [OK] Da cap nhat mon hoc " << code << ".\n";
            }

            else if (ch == 4) {
                std::string code = Utils::inputLine("  Nhap ma mon can xoa: ");
                if (!sbm.findByCode(code)) { std::cout << "  [!] Khong tim thay.\n"; continue; }
                std::cout << "  Xac nhan xoa " << code << "? (y/n): ";
                std::string cf; std::getline(std::cin, cf);
                if (Utils::trim(cf) == "y" || Utils::trim(cf) == "Y") {
                    sbm.remove(code);
                    FileManager::saveSubjects("subjects.txt", sbm);
                    std::cout << "  [OK] Da xoa mon hoc " << code << ".\n";
                } else std::cout << "  Da huy.\n";
            }

            else if (ch == 5) {
                std::string code = Utils::inputLine("  Nhap ma mon: ");
                SubNode* n = sbm.findByCode(code);
                if (!n) { std::cout << "  [!] Khong tim thay.\n"; continue; }
                Utils::line();
                std::cout << "  Ma mon   : " << n->data.code    << "\n"
                          << "  Ten mon  : " << n->data.name    << "\n"
                          << "  Tin chi  : " << n->data.credits << "\n";
                Utils::line();
            }
            else std::cout << "  [!] Lua chon khong hop le.\n";
            std::cout << "\n";
        }
    }

    // ============================================================
    //  MENU 3: ĐIỂM SỐ
    // ============================================================
    void menuScore() {
        while (true) {
            Utils::title("QUAN LY DIEM SO");
            std::cout << "  [1] Nhap / Cap nhat diem\n"
                      << "  [2] Xoa diem\n"
                      << "  [3] Xem tat ca ban ghi diem\n"
                      << "  [0] Quay lai\n";
            Utils::line();
            int ch = Utils::inputInt("  > Chon: ");
            std::cout << "\n";
            if (ch == 0) break;

            else if (ch == 1) {
                Utils::title("NHAP / CAP NHAT DIEM");
                std::string sid = Utils::inputLine("  MSSV sinh vien : ");
                if (!stm.findById(sid)) {
                    std::cout << "  [!] Khong tim thay sinh vien.\n"; continue;
                }
                std::string sub = Utils::inputLine("  Ma mon hoc     : ");
                if (!sbm.findByCode(sub)) {
                    std::cout << "  [!] Khong tim thay mon hoc.\n"; continue;
                }
                float s10 = Utils::inputFloat("  Diem he 10 (0-10): ", 0.0f, 10.0f);
                float g4  = Utils::toGPA4(s10);
                scm.addOrUpdate(sid, sub, s10, stm, sbm);
                FileManager::saveScores("scores.txt", scm);
                std::cout << "  [OK] Da luu diem: " << s10
                          << " (He 4: " << Utils::f2(g4)
                          << ", Chu: " << Utils::toLetter(g4) << ")\n";
            }

            else if (ch == 2) {
                std::string sid = Utils::inputLine("  MSSV           : ");
                std::string sub = Utils::inputLine("  Ma mon hoc     : ");
                if (scm.remove(sid, sub)) {
                    FileManager::saveScores("scores.txt", scm);
                    std::cout << "  [OK] Da xoa ban ghi diem.\n";
                } else {
                    std::cout << "  [!] Khong tim thay ban ghi diem.\n";
                }
            }

            else if (ch == 3) {
                Utils::title("TOAN BO BAN GHI DIEM");
                if (!scm.getHead()) {
                    std::cout << "  (Chua co ban ghi diem nao)\n";
                } else {
                    std::cout << Utils::col("MSSV", 13)
                              << Utils::col("Ma mon", 10)
                              << Utils::col("Diem 10", 9)
                              << Utils::col("Diem 4", 8)
                              << Utils::col("Chu", 5) << "\n";
                    Utils::line(48);
                    for (ScNode* c = scm.getHead(); c; c = c->next) {
                        std::cout << Utils::col(c->data.studentId,   13)
                                  << Utils::col(c->data.subjectCode, 10)
                                  << std::setw(7) << std::fixed << std::setprecision(1)
                                  << c->data.score10 << "  "
                                  << std::setw(5) << c->data.score4 << "  "
                                  << Utils::col(c->data.letter, 5) << "\n";
                    }
                    Utils::line(48);
                }
            }
            else std::cout << "  [!] Lua chon khong hop le.\n";
            std::cout << "\n";
        }
    }

    // ============================================================
    //  MENU 4: BÁO CÁO & TÌM KIẾM
    // ============================================================
    void menuReport() {
        while (true) {
            Utils::title("BAO CAO & TIM KIEM");
            std::cout << "  [1] Bang diem cua 1 sinh vien\n"
                      << "  [2] Bang diem lop hoc phan (theo ma mon)\n"
                      << "  [3] Bang xep hang GPA tich luy (giam dan)\n"
                      << "  [0] Quay lai\n";
            Utils::line();
            int ch = Utils::inputInt("  > Chon: ");
            std::cout << "\n";
            if (ch == 0) break;

            else if (ch == 1) {
                std::string sid = Utils::inputLine("  Nhap MSSV: ");
                scm.printTranscript(sid, stm, sbm);
            }

            else if (ch == 2) {
                std::string sub = Utils::inputLine("  Nhap ma mon hoc: ");
                scm.printClassReport(sub, stm, sbm);
            }

            else if (ch == 3) {
                scm.printRankingByGPA(stm, sbm);
            }

            else std::cout << "  [!] Lua chon khong hop le.\n";
            std::cout << "\n";
        }
    }

public:
    Menu(StudentManager& s, SubjectManager& sub, ScoreManager& sc)
        : stm(s), sbm(sub), scm(sc) {}

    // Vòng lặp chính của chương trình, hiển thị menu và điều hướng tác vụ – chạy đến khi người dùng chọn 0
    void run() {
        Utils::title("CHAO MUNG DEN HE THONG QUAN LY SINH VIEN");
        std::cout << "  Du lieu duoc tu dong tai tu file text.\n\n";

        while (true) {
            showMain();
            int ch = Utils::inputInt("  > Chon: ");
            std::cout << "\n";
            if      (ch == 1) menuStudent();
            else if (ch == 2) menuSubject();
            else if (ch == 3) menuScore();
            else if (ch == 4) menuReport();
            else if (ch == 0) {
                Utils::title("TAM BIET! Du lieu da duoc luu tu dong.");
                break;
            }
            else std::cout << "  [!] Lua chon khong hop le. Vui long thu lai.\n\n";
        }
    }
};