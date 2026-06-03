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

int main() {
    // Khởi tạo 3 manager trung tâm của hệ thống
    StudentManager stm;
    SubjectManager sbm;
    ScoreManager   scm;

    // Tải toàn bộ dữ liệu từ các file text (.txt) vào bộ nhớ
    FileManager::loadAll(stm, sbm, scm);

    // Khởi chạy vòng lặp menu điều khiển chính của console
    Menu menu(stm, sbm, scm);
    menu.run();

    // Lưu lại toàn bộ dữ liệu từ bộ nhớ xuống file trước khi thoát
    FileManager::saveAll(stm, sbm, scm);

    return 0;
}