// =============================================================================================
//  MENU.H – HỆ THỐNG ĐIỀU KHIỂN GIAO DIỆN
//  – Điều hướng người dùng qua 4 module chính
//  – Xử lý logic nhập liệu và gọi các hàm nghiệp vụ của các Manager
// =============================================================================================

#pragma once
#include "StudentManager.h"
#include "SubjectManager.h"
#include "ClassManager.h"
#include "ScoreManager.h"
#include "FileManager.h"
#include "Utils.h"
#include <iostream>
#include <string>

class Menu {
    StudentManager& stm;
    SubjectManager& sbm;
    ScoreManager&   scm;
    ClassManager&   cm;

    // Hiển thị giao diện menu chính lên console
    void showMain() const {
        Utils::title("              HỆ THỐNG QUẢN LÝ SINH VIÊN & ĐIỂM SỐ");
        std::cout << "  [1]  Quản lý Sinh viên\n"
                  << "  [2]  Quản lý Học phần\n"
                  << "  [3]  Quản lý Điểm số\n"
                  << "  [4]  Báo cáo & Tìm kiếm\n"
                  << "  [0]  Thoát chương trình\n";
        Utils::dline();
    }

    // ============================================================
    //  MENU 1: SINH VIÊN
    // ============================================================
    void menuStudent() {
        while (true) {
            Utils::title("QUẢN LÝ SINH VIÊN");
            std::cout << "  [1] Xem danh sách sinh viên\n"
                      << "  [2] Thêm sinh viên mới\n"
                      << "  [3] Sửa thông tin sinh viên\n"
                      << "  [4] Xóa sinh viên\n"
                      << "  [5] Tìm kiếm theo MSSV\n"
                      << "  [6] Tìm kiếm theo họ tên\n"
                      << "  [7] Sắp xếp theo MSSV\n"
                      << "  [8] Sắp xếp theo GPA\n"
                      << "  [9] Sắp xếp theo họ tên\n"
                      << "  [0] Quay lại\n";
            Utils::line();
            int ch = Utils::inputInt("  > Chọn: ");
            std::cout << "\n";

            if (ch == 0) break;

            else if (ch == 1) {
                Utils::title("    DANH SÁCH SINH VIÊN");
                stm.printAll();
            }

            else if (ch == 2) {
                Utils::title("    THÊM SINH VIÊN MỚI");
                Student s;

                // Nhập và kiểm tra MSSV
                while (true) {
                    s.id = Utils::inputRequiredLine("  MSSV         : ", "MSSV");
                    if (!Utils::isValidID(s.id)) std::cout << "  [!] LỖI: MSSV không hợp lệ.\n";
                    else if (stm.findById(s.id)) std::cout << "  [!] LỖI: MSSV đã tồn tại.\n";
                    else break;
                }

                // Nhập và kiểm tra Họ tên
                while (true) {
                    s.name = Utils::inputRequiredLine("  Họ và tên    : ", "Họ và tên");
                    if (!Utils::isValidName(s.name)) std::cout << "  [!] LỖI: Tên sinh viên không hợp lệ.\n";
                    else break;
                }

                // Nhập và kiểm tra Lớp, Ngày sinh, Ngành học, Email
                s.className = Utils::inputRequiredLine("  Lớp          : ", "Lớp");
                s.dob       = Utils::inputRequiredLine("  Ngày sinh    : ", "Ngày sinh");
                s.major     = Utils::inputRequiredLine("  Ngành học    : ", "Ngành học");
                s.email     = Utils::inputRequiredLine("  Email        : ", "Email");

                std::cout << "  Trạng thái (1-Đang học / 2-Bảo lưu / 3-Cảnh cáo / 4-Tốt nghiệp): ";
                int st = Utils::inputInt("");
                if      (st == 1) s.status = "Đang học";
                else if (st == 2) s.status = "Bảo lưu";
                else if (st == 3) s.status = "Cảnh cáo";
                else              s.status = "Tốt nghiệp";
                
                if (stm.add(s)) {
                    FileManager::saveStudents("students.txt", stm);
                    std::cout << "  [OK] Đã thêm sinh viên " << s.id << " thành công.\n";
                } else {
                    std::cout << "  [!] Có lỗi xảy ra khi thêm sinh viên.\n";
                }
            }

            else if (ch == 3) {
                Utils::title("    SỬA THÔNG TIN SINH VIÊN");
                std::string id = Utils::inputLine("  Nhập MSSV cần sửa: ");
                SNode* node = stm.findById(id);
                if (!node) { std::cout << "  [!] Không tìm thấy sinh viên có MSSV: " << id << "\n"; continue; }
                
                std::cout << "  (Nhấn Enter để giữ nguyên giá trị cũ)\n";
                Student s = node->data;
                std::string tmp;
                
                // Sửa Họ tên sinh viên
                while (true) {
                    tmp = Utils::inputLine("  Họ và tên    [" + s.name + "]: ");
                    if (tmp.empty()) break;
                    if (!Utils::isValidName(tmp)) {
                        std::cout << "  [!] LỖI: Tên không hợp lệ. Vui lòng nhập lại.\n";
                        continue;
                    }
                    s.name = tmp;
                    break;
                }

                // Sửa Lớp, Ngày sinh, Ngành học, Email, Trạng thái
                tmp = Utils::inputLine("  Lớp          [" + s.className + "]: "); 
                if (!tmp.empty() && !Utils::trim(tmp).empty()) s.className = Utils::trim(tmp);
                tmp = Utils::inputLine("  Ngày sinh    [" + s.dob       + "]: "); 
                if (!tmp.empty() && !Utils::trim(tmp).empty()) s.dob = Utils::trim(tmp);
                tmp = Utils::inputLine("  Ngành học    [" + s.major     + "]: "); 
                if (!tmp.empty() && !Utils::trim(tmp).empty()) s.major = Utils::trim(tmp);
                tmp = Utils::inputLine("  Email        [" + s.email     + "]: "); 
                if (!tmp.empty() && !Utils::trim(tmp).empty()) s.email = Utils::trim(tmp);
                
                std::cout << "  Trạng thái hiện tại [" << s.status << "]. Nhập mới (1-Đang học / 2-Bảo lưu / 3-Cảnh cáo / 4-Tốt nghiệp): ";
                int st = Utils::inputInt("");
                if      (st == 1) s.status = "Đang học";
                else if (st == 2) s.status = "Bảo lưu";
                else if (st == 3) s.status = "Cảnh cáo";
                else if (st == 4) s.status = "Tốt nghiệp";

                stm.update(id, s);
                FileManager::saveStudents("students.txt", stm);
                std::cout << "  [OK] Đã cập nhật sinh viên " << id << " thành công.\n";
            }

            else if (ch == 4) {
                Utils::title("      XOÁ SINH VIÊN");
                std::string id = Utils::inputLine("  Nhập MSSV cần xóa: ");
                if (!stm.findById(id)) { std::cout << "  [!] Không tìm thấy sinh viên có MSSV: " << id << "\n"; continue; }
                std::cout << "  Xác nhận xóa sinh viên " << id << "? (y/n): ";
                std::string cf; std::getline(std::cin, cf);
                if (Utils::trim(cf) == "y" || Utils::trim(cf) == "Y") {
                    stm.remove(id);
                    FileManager::saveStudents("students.txt", stm);
                    std::cout << "  [OK] Đã xoá sinh viên " << id << ".\n";
                } else {
                    std::cout << "  Đã huỷ thao tác.\n";
                }
            }

            else if (ch == 5) {
                std::string id = Utils::inputLine("  Nhập MSSV: ");
                SNode* n = stm.findById(id);
                if (!n) { std::cout << "  [!] Không tìm thấy sinh viên có MSSV: " << id << "\n"; continue; }
                Utils::line();
                std::cout << "  MSSV       : " << n->data.id        << "\n"
                          << "  Họ và tên  : " << n->data.name      << "\n"
                          << "  Lớp        : " << n->data.className << "\n"
                          << "  Ngày sinh  : " << n->data.dob       << "\n"
                          << "  Ngành học  : " << n->data.major     << "\n"
                          << "  Email      : " << n->data.email     << "\n"
                          << "  Trạng thái : " << n->data.status  << "\n";
                Utils::line();
            }

            else if (ch == 6) {
                std::string kw = Utils::inputLine("  Nhập từ khoá họ và tên: ");
                if (!stm.findByName(kw))
                    std::cout << "  [!] Không tìm thấy sinh viên nào.\n";
            }

            else if (ch == 7) {
                stm.sortById();
                FileManager::saveStudents("students.txt", stm);
                std::cout << "  [OK] Đã sắp xếp theo MSSV.\n";
                stm.printAll();
            }

            else if (ch == 8) {
                scm.printRankingByGPA(stm, sbm);
                std::cout << "  [OK] Đã sắp xếp theo GPA.\n";
            }

            else if (ch == 9) {
                stm.sortByName();
                FileManager::saveStudents("students.txt", stm);
                std::cout << "  [OK] Đã sắp xếp theo họ tên.\n";
                stm.printAll();
            }

            else {
                std::cout << "  [!] Lựa chọn không hợp lệ.\n";
            }
            std::cout << "\n";
        }
    }

    // ============================================================
    //  MENU 2: MÔN HỌC
    // ============================================================
    void menuSubject() {
        while (true) {
            Utils::title("     QUẢN LÝ HỌC PHẦN");
            std::cout << "  [1] Xem danh sách học phần\n"
                      << "  [2] Thêm học phần mới\n"
                      << "  [3] Sửa thông tin học phần\n"
                      << "  [4] Xoá học phần\n"
                      << "  [5] Tìm kiếm theo mã học phần\n"
                      << "  [6] Tìm kiếm theo tên học phần\n"
                      << "  -------- LỚP HỌC PHẦN --------\n"
                      << "  [7] Thêm Lớp học phần mới\n"
                      << "  [8] Thêm Sinh viên vào Lớp học phần\n"
                      << "  [9] Xem danh sách Sinh viên trong Lớp\n"
                      << "  [0] Quay lại\n";
            Utils::line();
            int ch = Utils::inputInt("  > Chọn: ");
            std::cout << "\n";
            if (ch == 0) break;

            else if (ch == 1) {
                Utils::title("     DANH SÁCH HỌC PHẦN");
                sbm.printAll();
            }

            else if (ch == 2) {
                Utils::title("     THÊM HỌC PHẦN MỚI");
                Subject s;

                // Nhập và kiểm tra Mã học phần
                while (true) {
                    s.code = Utils::inputRequiredLine("  Mã học phần  : ", "Mã học phần");
                    if (sbm.findByCode(s.code)) std::cout << "  [!] LỖI: Mã học phần đã tồn tại.\n";
                    else break;
                }

                // Nhập và kiểm tra Tên học phần (Không số/Ký tự đặc biệt)
                while (true) {
                    s.name = Utils::inputRequiredLine("  Tên học phần : ", "Tên học phần");
                    if (!Utils::isValidName(s.name)) std::cout << "  [!] LỖI: Tên học phần không hợp lệ.\n";
                    else break;
                }

                // Nhập và kiểm tra Số tín chỉ
                while (true) {
                    s.credits = Utils::inputInt("  Số tín chỉ  : ");
                    if (s.credits <= 0) std::cout << "  [!] LỖI: Số tín chỉ không hợp lệ.\n";
                    else break;
                }

                sbm.add(s);
                FileManager::saveSubjects("subjects.txt", sbm);
                std::cout << "  [OK] Đã thêm học phần " << s.code << " thành công.\n";
            }

            else if (ch == 3) {
                Utils::title("      SỬA HỌC PHẦN");
                std::string code = Utils::inputLine("  Nhập mã học phần cần sửa: ");
                SubNode* node = sbm.findByCode(code);
                if (!node) { std::cout << "  [!] Không tìm thấy mã học phần: " << code << "\n"; continue; }
                
                std::cout << "  (Nhấn Enter để giữ nguyên giá trị cũ)\n";
                Subject s = node->data;
                std::string tmp;

                // Sửa Tên học phần
                while (true) {
                    tmp = Utils::inputLine("  Tên học phần [" + s.name + "]: ");
                    if (tmp.empty()) break;
                    tmp = Utils::trim(tmp);
                    if (!Utils::isValidName(tmp)) {
                        std::cout << "  [!] LỖI: Tên học phần không hợp lệ. Vui lòng nhập lại.\n";
                        continue;
                    }
                    s.name = tmp;
                    break;
                }

                // Sửa Số tín chỉ
                while (true) {
                    std::cout << "  Số tín chỉ   [" << s.credits << "]: ";
                    std::string tc; 
                    std::getline(std::cin, tc); 
                    tc = Utils::trim(tc);
                    
                    if (tc.empty()) break;
                    
                    try {
                        int newCredits = std::stoi(tc);
                        if (newCredits <= 0)
                            std::cout << "  [!] LỖI: Số tín chỉ không hợp lệ. Vui lòng nhập lại.\n";
                        else {
                            s.credits = newCredits;
                            break;
                        }
                    } catch (...) {
                        std::cout << "  [!] LỖI: Số tín chỉ không hợp lệ. Vui lòng nhập lại.\n";
                    }
                }

                sbm.update(code, s);
                FileManager::saveSubjects("subjects.txt", sbm);
                std::cout << "  [OK] Đã cập nhật học phần " << code << " thành công.\n";
            }

            else if (ch == 4) {
                std::string code = Utils::inputLine("  Nhập mã học phần cần xoá: ");
                if (!sbm.findByCode(code)) { std::cout << "  [!] Không tìm thấy mã học phần: " << code << "\n"; continue; }
                
                // Cảnh báo nếu đã có dữ liệu điểm
                if (scm.hasScoreForSubject(code)) {
                    std::cout << "  [!] CẢNH BÁO: Học phần này đã có bản ghi điểm sinh viên.\n";
                    std::cout << "  Bạn vẫn muốn xoá học phần này? (y/n): ";
                } else
                    std::cout << "  Xác nhận xóa học phần " << code << "? (y/n): ";
                std::string cf; std::getline(std::cin, cf);
                if (Utils::trim(cf)=="y" || Utils::trim(cf)=="Y") {
                    sbm.remove(code);
                    FileManager::saveSubjects("subjects.txt", sbm);
                    std::cout << "  [OK] Đã xoá học phần " << code << ".\n";
                } else
                    std::cout << "  Đã huỷ thao tác.\n";
            }

            else if (ch == 5) {
                std::string code = Utils::inputLine("  Nhập mã học phần: ");
                SubNode* n = sbm.findByCode(code);
                if (!n) { std::cout << "  [!] Không tìm thấy mã học phần: " << code << "\n"; continue; }
                Utils::line();
                std::cout << "  Mã học phần   : " << n->data.code    << "\n"
                          << "  Tên học phần  : " << n->data.name    << "\n"
                          << "  Số tín chỉ    : " << n->data.credits << "\n";
                Utils::line();
            }

            else if (ch == 6) {
                std::string name = Utils::inputLine("  Nhập từ khoá tên học phần: ");
                if (!sbm.findByName(name))
                    std::cout << "  [!] Không tìm thấy học phần nào có tên chứa: " << name << "\n";
            }

            else if (ch == 7) {
                Utils::title("    THÊM LỚP HỌC PHẦN MỚI");
                ClassSession cs;

                while (true) {
                    cs.subjectCode = Utils::inputRequiredLine("  Nhập Mã học phần: ", "Mã học phần");
                    if (!sbm.findByCode(cs.subjectCode))
                        std::cout << "  [!] LỖI: Mã học phần chưa tồn tại trong hệ thống. Hãy thêm ở Menu [2] trước.\n";
                    else break; 
                }

                while (true) {
                    cs.classCode = Utils::inputRequiredLine("  Nhập Mã lớp học phần: ", "Mã lớp học phần");
                    if (cm.findByClassCode(cs.classCode))
                        std::cout << "  [!] LỖI: Lớp học phần này đã tồn tại. Vui lòng nhập mã khác.\n";
                    else break;
                }
                
                if (cm.addClass(cs)) {
                    FileManager::saveClasses("classes.txt", "class_roster.txt", cm);
                    std::cout << "  [OK] Đã tạo lớp học phần " << cs.classCode << " cho môn " << cs.subjectCode << " thành công.\n";                }
            }

            else if (ch == 8) {
                Utils::title("    THÊM SINH VIÊN VÀO LỚP HỌC PHẦN");
                std::string cCode = Utils::inputLine("  Nhập Mã lớp: ");
                std::string sId = Utils::inputLine("  Nhập MSSV: ");
                
                if (cm.addStudentToClass(cCode, sId, stm)) {
                    FileManager::saveClasses("classes.txt", "class_roster.txt", cm);
                    std::cout << "  [OK] Đã thêm sinh viên " << sId << " vào lớp " << cCode << ".\n";
                } else {
                    std::cout << "  [!] LỖI: Không thể thêm (Sai mã lớp, sinh viên không tồn tại, hoặc đã có trong lớp).\n";
                }
            }

            else if (ch == 9) {
                std::string cCode = Utils::inputLine("  Nhập Mã lớp cần xem: ");
                cm.printClassRoster(cCode, stm, sbm);
            }

            else std::cout << "  [!] Lựa chọn không hợp lệ.\n";
            std::cout << "\n";
        }
    }

    // ============================================================
    //  MENU 3: ĐIỂM SỐ
    // ============================================================
    void menuScore() {
        while (true) {
            Utils::title("      QUẢN LÝ ĐIỂM SỐ");
            std::cout << "  [1] Nhập / Cập nhật điểm\n"
                      << "  [2] Xoá điểm\n"
                      << "  [3] Xem tất cả bản ghi điểm\n"
                      << "  [0] Quay lại\n";
            Utils::line();
            int ch = Utils::inputInt("  > Chọn: ");
            std::cout << "\n";
            if (ch == 0) break;

            else if (ch == 1) {
                Utils::title("     NHẬP / CẬP NHẬT ĐIỂM");
                std::string classCode = Utils::inputLine("  Nhập Mã lớp học phần : ");
                ClassNode* cn = cm.findByClassCode(classCode);
                if (!cn) {
                    std::cout << "  [!] Không tìm thấy lớp học phần.\n"; 
                    continue;
                }
                std::string subCode = cn->data.subjectCode;
                SubNode* subNode = sbm.findByCode(subCode);
                std::string subName = subNode ? subNode->data.name : "  [!] Không tìm thấy học phần.";
                std::cout << "  => Học phần: " << subCode << " - " << subName << "\n";

                std::string sid = Utils::inputLine("  Nhập MSSV            : ");
                
                // Kiểm tra sinh viên có đăng ký Lớp học phần này không
                bool isInClass = false;
                for (RosterNode* r = cn->data.rosterHead; r; r = r->next) {
                    if (r->studentId == sid) {
                        isInClass = true;
                        break;
                    }
                }

                if (!isInClass) {
                    std::cout << "  [!] Sinh viên " << sid << " không có trong danh sách Lớp học phần " << classCode << ".\n";
                    std::cout << "  [!] Vui lòng thêm sinh viên vào lớp trước khi nhập điểm.\n";
                    continue;
                }

                float a, b;
                while (true) {
                    a = Utils::inputFloat("  Nhập trọng số chuyên cần [0.0 - 1.0]: ", 0.0f, 1.0f);
                    b = Utils::inputFloat("  Nhập trọng số giữa kỳ    [0.0 - 1.0]: ", 0.0f, 1.0f);
                    
                    // Kiểm tra tổng trọng số
                    if (a + b != 1.0f) std::cout << "  [!] LỖI: Trọng số không hợp lệ. Vui lòng nhập lại.\n";
                    else break;
                }

                float cc = Utils::inputFloat("  Điểm chuyên cần (0-10): ", 0.0f, 10.0f);
                float gk = Utils::inputFloat("  Điểm giữa kỳ    (0-10): ", 0.0f, 10.0f);
                float ck = Utils::inputFloat("  Điểm cuối kỳ    (0-10): ", 0.0f, 10.0f);
                
                float s10 = Utils::calculateFinalScore(cc, gk, ck, a, b);
                std::cout << "  => Điểm hệ 10: " << s10 << "\n";
                float g4  = Utils::toGPA4(s10);

                scm.addOrUpdate(sid, subCode, s10, stm, sbm);
                FileManager::saveScores("scores.txt", scm);
                std::cout << "  [OK] Đã lưu điểm: " << s10
                          << " (Hệ 4: " << Utils::f2(g4)
                          << ", Hệ chữ: " << Utils::toLetter(s10) << ")\n";
            }

            else if (ch == 2) {
                std::string sid = Utils::inputLine("  MSSV            : ");
                std::string sub = Utils::inputLine("  Mã môn học phần : ");
                if (scm.remove(sid, sub)) {
                    FileManager::saveScores("scores.txt", scm);
                    std::cout << "  [OK] Đã xoá bản ghi điểm.\n";
                } else {
                    std::cout << "  [!] Không tìm thấy bản ghi điểm.\n";
                }
            }

            else if (ch == 3) {
                Utils::title("     TOÀN BỘ BẢN GHI ĐIỂM");
                if (!scm.getHead())
                    std::cout << "  (Chưa có bản ghi điểm nào)\n";
                else {
                    std::cout << Utils::col("MSSV", 12)
                              << Utils::col("Mã học phần", 15)
                              << Utils::col("Điểm 10", 10)
                              << Utils::col("Điểm 4", 10)
                              << Utils::col("Điểm chữ", 5) << "\n";
                    Utils::line();

                    for (ScNode* c = scm.getHead(); c; c = c->next) {
                        std::cout << Utils::col(c->data.studentId,   12)
                                  << Utils::col(c->data.subjectCode, 15)
                                  << Utils::col(Utils::f2(c->data.score10), 10)
                                  << Utils::col(Utils::f2(c->data.score4), 10)
                                  << Utils::col(c->data.letter, 5) << "\n";
                    }
                    Utils::line();
                }
            }
            
            else std::cout << "  [!] Lựa chọn không hợp lệ.\n";
            std::cout << "\n";
        }
    }

    // ============================================================
    //  MENU 4: BÁO CÁO & TÌM KIẾM
    // ============================================================
    void menuReport() {
        while (true) {
            Utils::title("BÁO CÁO & TÌM KIẾM");
            std::cout << "  [1] Bảng điểm của 1 sinh viên\n"
                      << "  [2] Bảng điểm môn học phần\n"
                      << "  [3] Bảng xếp hạng GPA tích luỹ\n"
                      << "  [0] Quay lại\n";
            Utils::line();
            int ch = Utils::inputInt("  > Chọn: ");
            std::cout << "\n";
            if (ch == 0) break;

            else if (ch == 1) {
                std::string sid = Utils::inputLine("  Nhập MSSV: ");
                scm.printTranscript(sid, stm, sbm);
            }

            else if (ch == 2) {
                std::string sub = Utils::inputLine("  Nhập mã môn học phần: ");
                scm.printClassReport(sub, stm, sbm);
            }

            else if (ch == 3) {
                scm.printRankingByGPA(stm, sbm);
            }

            else std::cout << "  [!] Lựa chọn không hợp lệ.\n";
            std::cout << "\n";
        }
    }

public:
    Menu(StudentManager& s, SubjectManager& sub, ScoreManager& sc, ClassManager& c)
        : stm(s), sbm(sub), scm(sc), cm(c) {}

    // Vòng lặp chính của chương trình, hiển thị menu và điều hướng tác vụ – chạy đến khi người dùng chọn 0
    void run() {
        Utils::title("             CHÀO MỪNG ĐẾN HỆ THỐNG QUẢN LÝ SINH VIÊN");
        std::cout << "  Dữ liệu được tự động tải từ file text.\n\n";

        while (true) {
            showMain();
            int ch = Utils::inputInt("  > Chon: ");
            std::cout << "\n";
            if      (ch == 1) menuStudent();
            else if (ch == 2) menuSubject();
            else if (ch == 3) menuScore();
            else if (ch == 4) menuReport();
            else if (ch == 0) {
                Utils::title("TẠM BIỆT! Dữ liệu đã được lưu tự động.");
                break;
            }
            else std::cout << "  [!] Lựa chọn không hợp lệ, Vui lòng thử lại.\n\n";
        }
    }
};