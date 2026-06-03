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
#include "ScoreManager.h"
#include "Utils.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

namespace FileManager {

const char SEP = '|';

// Tách chuỗi dữ liệu từ file dựa trên ký tự phân cách (separator)
inline void split(const std::string& line, char sep,
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

// Lưu sinh viên
inline bool saveStudents(const std::string& path, const StudentManager& sm) {
    std::ofstream f(path);
    if (!f.is_open()) return false;
    for (SNode* c = sm.getHead(); c; c = c->next) {
        const Student& s = c->data;
        f << s.id     << SEP << s.name  << SEP << s.className << SEP
          << s.dob    << SEP << s.major << SEP << s.email     << SEP
          << s.status << "\n";
    }
    return true;
}

// Tải danh sách sinh viên từ file "students.txt"
inline bool loadStudents(const std::string& path, StudentManager& sm) {
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

// Lưu học phần
inline bool saveSubjects(const std::string& path, const SubjectManager& sm) {
    std::ofstream f(path);
    if (!f.is_open()) return false;
    for (SubNode* c = sm.getHead(); c; c = c->next) {
        const Subject& s = c->data;
        f << s.code << SEP << s.name << SEP << s.credits << "\n";
    }
    return true;
}

// Tải danh sách học phần từ file "subjects.txt"
inline bool loadSubjects(const std::string& path, SubjectManager& sm) {
    std::ifstream f(path);
    if (!f.is_open()) return false;
    std::string line;
    while (std::getline(f, line)) {
        line = Utils::trim(line);
        if (line.empty()) continue;
        std::string p[3]; int got;
        split(line, SEP, p, 3, got);
        if (got < 3) continue;
        Subject s;
        s.code = p[0]; s.name = p[1];
        try { s.credits = std::stoi(p[2]); } catch (...) { s.credits = 0; }
        sm.add(s);
    }
    return true;
}

// Lưu điểm số
inline bool saveScores(const std::string& path, const ScoreManager& scm) {
    std::ofstream f(path);
    if (!f.is_open()) return false;
    for (ScNode* c = scm.getHead(); c; c = c->next) {
        f << c->data.studentId  << SEP
          << c->data.subjectCode << SEP
          << std::fixed << std::setprecision(1) << c->data.score10 << "\n";
    }
    return true;
}

// Tải danh sách điểm số từ file "scores.txt"
inline bool loadScores(const std::string& path,
                       ScoreManager& scm,
                       const StudentManager& stm,
                       const SubjectManager& sbm) {
    std::ifstream f(path);
    if (!f.is_open()) return false;
    std::string line;
    while (std::getline(f, line)) {
        line = Utils::trim(line);
        if (line.empty()) continue;
        std::string p[3]; int got;
        split(line, SEP, p, 3, got);
        if (got < 3) continue;
        try {
            float s10 = std::stof(p[2]);
            scm.addOrUpdate(p[0], p[1], s10, stm, sbm);
        } catch (...) {}
    }
    return true;
}

// Đồng bộ hóa toàn bộ dữ liệu (Sinh viên, Học phần, Điểm số) với file text
inline void loadAll(StudentManager& stm, SubjectManager& sbm, ScoreManager& scm) {
    loadStudents("students.txt", stm);
    loadSubjects("subjects.txt", sbm);
    loadScores  ("scores.txt",   scm, stm, sbm);
}

inline void saveAll(const StudentManager& stm,
                    const SubjectManager& sbm,
                    const ScoreManager&   scm) {
    saveStudents("students.txt", stm);
    saveSubjects("subjects.txt", sbm);
    saveScores  ("scores.txt",   scm);
}

}