// =============================================================================================
//  FILEMANAGER.H – ĐỌC / GHI DỮ LIỆU
//  – Xử lý lưu trữ dữ liệu ra file text.
//  – Định dạng file: các trường cách nhau bằng dấu '|'.
//
//  students.txt  : id|name|class|dob|major|email|status
//  subjects.txt  : code|name|credits
//  scores.txt    : studentId|subjectCode|score10
// =============================================================================================

#pragma once
#include "StudentManager.h"
#include "SubjectManager.h"
#include "ClassManager.h"
#include "ScoreManager.h"
#include "Utils.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

class FileManager {
public:
    static const char SEP = '|';

    // Tách chuỗi dữ liệu từ file dựa trên ký tự phân cách
    static void split(const std::string& line, char sep,
                    std::string out[], int maxParts, int& got) {
        got = 0;
        std::string cur;
        for (char c : line) {
            if (c == sep) {
                if (got < maxParts) out[got++] = Utils::trim(cur);
                cur.clear();
            } else {
                cur += c;
            }
        }
        if (got < maxParts) out[got++] = Utils::trim(cur);
    }

    // ==================================================================
    // QUẢN LÝ ĐỌC/GHI SINH VIÊN
    // ==================================================================
    static bool saveStudents(const std::string& path, const StudentManager& sm) {
        std::ofstream f(path);
        if (!f.is_open()) return false;
        // Bắt buộc dùng const SNode* vì đã áp dụng Encapsulation
        for (const SNode* c = sm.head; c; c = c->next) {
            const Student& s = c->data;
            f << s.id     << SEP << s.name  << SEP << s.className << SEP
              << s.dob    << SEP << s.major << SEP << s.email     << SEP
              << s.status << "\n";
        }
        return true;
    }

    static bool loadStudents(const std::string& path, StudentManager& sm) {
        std::ifstream f(path);
        if (!f.is_open()) return false;
        std::string line;
        while (std::getline(f, line)) {
            line = Utils::trim(line);
            if (line.empty()) continue;
            std::string p[7]; int got;
            split(line, SEP, p, 7, got);
            if (got < 7) continue;
            Student s;
            s.id = p[0]; s.name = p[1]; s.className = p[2];
            s.dob = p[3]; s.major = p[4]; s.email = p[5]; s.status = p[6];
            sm.add(s);
        }
        return true;
    }

    // ==================================================================
    // QUẢN LÝ ĐỌC/GHI HỌC PHẦN
    // ==================================================================
    static bool saveSubjects(const std::string& path, const SubjectManager& sm) {
        std::ofstream f(path);
        if (!f.is_open()) return false;
        for (const SubNode* c = sm.head; c; c = c->next) {
            const Subject& s = c->data;
            f << s.code << SEP << s.name << SEP << s.credits << SEP << (s.isActive ? 1 : 0) << "\n";
        }
        return true;
    }

    static bool loadSubjects(const std::string& path, SubjectManager& sm) {
        std::ifstream f(path);
        if (!f.is_open()) return false;
        std::string line;
        while (std::getline(f, line)) {
            line = Utils::trim(line);
            if (line.empty()) continue;

            std::string p[4]; int got;
            split(line, SEP, p, 4, got);
            if (got < 3) continue;

            Subject s;
            s.code = p[0]; s.name = p[1];
            try { 
                s.credits = std::stoi(p[2]); 
            } catch (const std::exception& e) { 
                std::cerr << "  [!] Cảnh báo: Lỗi format tín chỉ ở học phần " << s.code << ". Đã gán = 0.\n";
                s.credits = 0; 
            }

            if (got >= 4) s.isActive = (p[3] == "1");
            else s.isActive = true; 

            sm.add(s);
        }
        return true;
    }

    // ==================================================================
    // QUẢN LÝ ĐỌC/GHI LỚP HỌC PHẦN
    // ==================================================================
    static bool saveClasses(const std::string& classPath, const std::string& rosterPath, const ClassManager& cm) {        
        std::ofstream fc(classPath);
        std::ofstream fr(rosterPath);
        if (!fc.is_open() || !fr.is_open()) return false;

        for (const ClassNode* c = cm.head; c; c = c->next) {
            const ClassSession& cs = c->data;
            fc << cs.classCode << SEP << cs.subjectCode << SEP << cs.semester << SEP
               << cs.wCC << SEP << cs.wTC << SEP << cs.wGK << SEP << cs.wFinal << "\n";
            
            for (RosterNode* r = cs.rosterHead; r; r = r->next)
                fr << cs.classCode << SEP << r->studentId << "\n";
        }
        return true;
    }

    static bool loadClasses(const std::string& classPath, const std::string& rosterPath, ClassManager& cm, const StudentManager& stm) {
        std::ifstream fc(classPath);
        if (fc.is_open()) {
            std::string line;
            while (std::getline(fc, line)) {
                line = Utils::trim(line);
                if (line.empty()) continue;

                std::string p[7]; int got;
                split(line, SEP, p, 7, got);
                if (got < 3) continue;

                ClassSession cs;
                cs.classCode   = p[0];
                cs.subjectCode = p[1];
                cs.semester    = p[2];

                if (got >= 7) {
                    try {
                        cs.wCC = std::stof(p[3]);
                        cs.wTC = std::stof(p[4]);
                        cs.wGK = std::stof(p[5]);
                        cs.wFinal = std::stof(p[6]);
                    } catch (...) {
                    }
                }
                cm.addClass(cs);
            }
        }

        std::ifstream fr(rosterPath);
        if (fr.is_open()) {
            std::string line;
            while (std::getline(fr, line)) {
                line = Utils::trim(line);
                if (line.empty()) continue;
                std::string p[2]; int got;
                split(line, SEP, p, 2, got);
                if (got < 2) continue;
                std::string classCode = p[0];
                std::string studentId = p[1];
                cm.addStudentToClass(classCode, studentId, stm);
            }
        }
        return true;
    }

    // ==================================================================
    // QUẢN LÝ ĐỌC/GHI ĐIỂM SỐ
    // ==================================================================
    static bool saveScores(const std::string& path, const ScoreManager& scm) {
        std::ofstream f(path);
        if (!f.is_open()) return false;
        for (const ScNode* c = scm.head; c; c = c->next) {
            f << c->data.studentId   << SEP
              << c->data.subjectCode << SEP
              << c->data.semester    << SEP
              << std::fixed << std::setprecision(1) << c->data.cc << SEP
              << std::fixed << std::setprecision(1) << c->data.tc << SEP
              << std::fixed << std::setprecision(1) << c->data.gk << SEP
              << std::fixed << std::setprecision(1) << c->data.ck << SEP
              << std::fixed << std::setprecision(1) << c->data.score10 << "\n";
        }
        return true;
    }

    static bool loadScores(const std::string& path,
                        ScoreManager& scm,
                        const StudentManager& stm,
                        const SubjectManager& sbm) {
        std::ifstream f(path);
        if (!f.is_open()) return false;
        std::string line;
        while (std::getline(f, line)) {
            line = Utils::trim(line);
            if (line.empty()) continue;
            std::string p[8]; int got;
            split(line, SEP, p, 8, got);
            if (got < 8) continue;
            try {
                float cc  = std::stof(p[3]);
                float tc  = std::stof(p[4]);
                float gk  = std::stof(p[5]);
                float ck  = std::stof(p[6]);
                float s10 = std::stof(p[7]);
                scm.addOrUpdate(p[0], p[1], p[2], cc, tc, gk, ck, s10, stm, sbm);
            } catch (const std::exception& e) {
                std::cerr << "  [!] Cảnh báo: Bỏ qua dòng điểm bị lỗi định dạng: " << line << "\n";
            }
        }
        return true;
    }

    // ==================================================================
    // HÀM ĐỒNG BỘ HÓA TỔNG THỂ
    // ==================================================================
    static void loadAll(StudentManager& stm, SubjectManager& sbm, ClassManager& cm, ScoreManager& scm) {
        loadStudents("data/students.txt", stm);
        loadSubjects("data/subjects.txt", sbm);
        loadClasses ("data/classes.txt", "data/class_roster.txt", cm, stm);
        loadScores  ("data/scores.txt",   scm, stm, sbm);
    }

    static void saveAll(const StudentManager& stm,
                        const SubjectManager& sbm,
                        const ClassManager&   cm,
                        const ScoreManager&   scm) {
        saveStudents("data/students.txt", stm);
        saveSubjects("data/subjects.txt", sbm);
        saveClasses ("data/classes.txt", "data/class_roster.txt", cm);
        saveScores  ("data/scores.txt",   scm);
    }
};
