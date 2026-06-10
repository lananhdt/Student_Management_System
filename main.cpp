// =============================================================================================
//  PROJECT  : HỆ THỐNG QUẢN LÝ SINH VIÊN & ĐIỂM SỐ
//  Ngôn ngữ : C++
//  Cấu trúc : Doubly Linked List, Hash Map, Merge Sort
//  Lưu trữ  : File text (students.txt / subjects.txt / scores.txt)
//  Mô tả    : Entry point của chương trình, thực hiện khởi tạo dữ liệu, 
//             điều hướng menu và lưu trữ dữ liệu trước khi thoát.
// =============================================================================================

#include "Menu.h"
#include "FileManager.h"
#include "ClassManager.h"
#ifdef _WIN32
#include <windows.h>
#endif

int main() {
    // Hiển thị Tiếng Việt trên Window
    #ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    #endif

    // Khởi tạo các manager trung tâm của hệ thống
    StudentManager stm;
    SubjectManager sbm;
    ClassManager   cm;
    ScoreManager   scm;

    // Tải toàn bộ dữ liệu từ các file text (.txt) vào bộ nhớ
    FileManager::loadAll(stm, sbm, cm, scm);

    // Khởi chạy vòng lặp menu điều khiển chính của console
    Menu menu(stm, sbm, scm, cm);
    menu.run();

    // Lưu lại toàn bộ dữ liệu từ bộ nhớ xuống file trước khi thoát
    FileManager::saveAll(stm, sbm, cm, scm);

    return 0;
}