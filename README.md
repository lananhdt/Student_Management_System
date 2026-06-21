# Hệ Thống Quản Lý Sinh Viên & Điểm Số

---

## Mục lục

1. [Mô tả tổng quan](#1-mô-tả-tổng-quan)
2. [Tính năng hệ thống](#2-tính-năng-hệ-thống)
3. [Yêu cầu hệ thống](#3-yêu-cầu-hệ-thống)
4. [Cấu trúc dự án](#4-cấu-trúc-dự-án)
5. [Cấu trúc dữ liệu & Thuật toán](#5-cấu-trúc-dữ-liệu--thuật-toán)
6. [Định dạng file dữ liệu](#6-định-dạng-file-dữ-liệu)
7. [Hướng dẫn cài đặt & Biên dịch](#7-hướng-dẫn-cài-đặt--biên-dịch)
8. [Logic nghiệp vụ](#8-logic-nghiệp-vụ)
9. [Lưu ý & Giới hạn](#9-lưu-ý--giới-hạn)

---

## 1. Mô tả tổng quan

Chương trình quản lý sinh viên và kết quả học tập được phát triển bằng **C++ thuần (chuẩn C++11)**, vận hành qua giao diện dòng lệnh (CLI). Toàn bộ cấu trúc dữ liệu và thuật toán được tự cài đặt từ đầu - không sử dụng thư viện cấu trúc dữ liệu có sẵn (như `std::list`, `std::map`, `std::sort`…). 

Hệ thống áp dụng kiến trúc phân tầng OOP chặt chẽ, tối ưu hóa tốc độ truy xuất bằng Bảng băm (Hash Map) và quản lý lưu trữ bền vững qua các tệp tin cục bộ.

---

## 2. Tính năng hệ thống

Hệ thống được chia thành 4 phân hệ (module) chính:

### 2.1 Quản lý Sinh viên
- Thêm mới, cập nhật thông tin sinh viên.
- **Xóa mềm (Soft Delete):** Đổi trạng thái thành "Đã nghỉ học", bảo toàn lịch sử dữ liệu.
- Tìm kiếm tức thời theo MSSV O(1) qua Hash Map.
- Tìm kiếm tương đối theo họ tên (String Matching).
- Sắp xếp danh sách theo MSSV hoặc Họ tên (Merge Sort O(N log N)).

### 2.2 Quản lý Học phần & Lớp học phần
- Quản lý danh mục Môn học: Thêm, sửa, vô hiệu hóa môn học.
- **Quản lý Lớp học phần:** Tạo lớp thực tế mở trong học kỳ, liên kết với môn học.
- Thiết lập trọng số đánh giá linh hoạt cho từng lớp (Chuyên cần, Tích cực, Giữa kỳ, Cuối kỳ).
- Thêm sinh viên vào danh sách lớp và xuất danh sách lớp.

### 2.3 Quản lý Điểm số
- Nhập điểm thành phần theo Lớp học phần.
- Hệ thống tự động nội suy điểm hệ 10 dựa trên trọng số của lớp.
- Tự động quy đổi điểm hệ 4 và điểm chữ chuẩn Bách Khoa.
- Cập nhật và xóa bản ghi điểm.

### 2.4 Báo cáo & Thống kê
- Trích xuất bảng điểm chi tiết của cá nhân, tự động tính **CPA tích lũy** và xếp loại học lực.
- Trích xuất bảng điểm của Lớp học phần, báo cáo phổ điểm (A+, A, B+...).
- Lập bảng xếp hạng GPA tích lũy toàn trường (Sắp xếp bằng Merge Sort trên Singly Linked List).

---

## 3. Yêu cầu hệ thống

### Phần cứng
- **CPU:** Kiến trúc x86 / x86-64 / ARM.
- **RAM:** Cực kỳ tối ưu, chỉ yêu cầu ≥ 16 MB.
- **Ổ đĩa:** Khoảng 5 MB trống cho thực thi và dữ liệu.
- **Hiển thị:** Terminal Console hỗ trợ mã hóa UTF-8.

### Phần mềm
- **Trình biên dịch:** `g++` (GCC ≥ 5.0) hoặc MSVC, hỗ trợ chuẩn C++11.
- **Thư viện:** Khai thác hoàn toàn trên C++ Standard Library (`<iostream>`, `<fstream>`, `<string>`, `<iomanip>`). Không yêu cầu Dependencies bên ngoài.

---

## 4. Cấu trúc dự án

Dự án áp dụng mô hình phân tách thư mục chuẩn mực, chia tách rõ ràng mã nguồn và tệp dữ liệu:

```text
student_management_system/
│
├── data/                 # Thư mục chứa CSDL (Bắt buộc phải tạo trước khi chạy)
│   ├── students.txt      # Lưu trữ danh sách Sinh viên
│   ├── subjects.txt      # Lưu trữ danh mục Môn học
│   ├── classes.txt       # Cấu hình Lớp học phần & Trọng số
│   ├── class_roster.txt  # Danh sách sinh viên đăng ký lớp
│   └── scores.txt        # Lưu trữ Điểm số
│
├── src/                  # Thư mục chứa Mã nguồn
│   ├── main.cpp          # Entry point điều hướng hệ thống
│   ├── Models.h          # Cấu trúc Entities & Nodes (SNode, SubNode, ClassNode...)
│   ├── Utils.h           # Core logic: Quy đổi điểm, Validate nhập liệu, Định dạng in
│   ├── HashMap.h         # Cấu trúc Bảng băm (StudentHashMap, SubjectHashMap)
│   ├── StudentManager.h  # Module xử lý nghiệp vụ Sinh viên
│   ├── SubjectManager.h  # Module xử lý nghiệp vụ Môn học
│   ├── ClassManager.h    # Module tổ chức Lớp học phần & Roster
│   ├── ScoreManager.h    # Module tính toán GPA & Báo cáo
│   ├── FileManager.h     # Giao tiếp I/O đọc/ghi tệp tin
│   └── Menu.h            # Vòng lặp điều hướng giao diện CLI
│
└── README.md
```

---

## 5. Cấu trúc dữ liệu & Thuật toán

### 5.1 Các cấu trúc dữ liệu tự cài đặt
- **Doubly Linked List (Danh sách liên kết đôi):** Lưu trữ thực thể chính (`SNode`, `SubNode`, `ScNode`, `ClassNode`). Cung cấp khả năng xóa/chèn với chi phí dời bộ nhớ là O(1) khi biết vị trí.
- **Singly Linked List (Danh sách liên kết đơn):** Quản lý danh sách lớp (`RosterNode`) và làm vùng nhớ đệm trung gian cho các tác vụ tính toán (`RankNode`, `MaxScoreNode`), ngăn chặn Memory Leak.
- **Hash Map (Bảng băm):** Triển khai thuật toán *Polynomial Rolling Hash*, giải quyết va chạm bằng *Separate Chaining*. Tích hợp cơ chế tự động mở rộng mảng (Rehashing) khi *Load Factor* đạt 75%. Đảm bảo truy xuất định danh O(1).

### 5.2 Thuật toán sắp xếp (Merge Sort)
Sử dụng tư duy "Con trỏ Nhanh - Chậm" (Slow-Fast Pointers) để cắt/nối danh sách liên kết bằng đệ quy. 
- **Ưu điểm:** Duy trì độ ổn định (Stable Sort), độ phức tạp thời gian **O(N log N)**, độ phức tạp không gian **O(1)** — không tiêu tốn RAM tạo mảng phụ.

---

## 6. Định dạng file dữ liệu

Hệ thống sử dụng ký tự phân cách `|` (Pipe) nhằm phân tách dữ liệu thô an toàn, tránh lỗi đứt gãy do dấu phẩy hay khoảng trắng.

- **`students.txt`:** `MSSV | Họ Tên | Lớp | Ngày Sinh | Ngành | Email | Trạng thái`
- **`subjects.txt`:** `Mã môn | Tên môn | Tín chỉ | Trạng thái (1/0)`
- **`classes.txt`:** `Mã lớp | Mã môn | Học kỳ | Trọng số CC | TS TC | TS GK | TS CK`
- **`class_roster.txt`:** `Mã lớp | MSSV`
- **`scores.txt`:** `MSSV | Mã môn | Học kỳ | Điểm CC | Điểm TC | Điểm GK | Điểm CK | Điểm hệ 10`

---

## 7. Hướng dẫn cài đặt & Biên dịch

### Bước 1: Chuẩn bị mã nguồn
Tải toàn bộ mã nguồn về máy. Đảm bảo thư mục gốc (root) có tồn tại thư mục rỗng mang tên `data`. (Nếu chưa có, hãy tạo mới folder `data` để hệ thống có nơi lưu file).

### Bước 2: Biên dịch
Mở terminal/cmd tại thư mục gốc của dự án.

**Windows (Sử dụng MinGW/G++):**
```bash
g++ -std=c++11 -o system src/main.cpp
```

**Linux / macOS:**
```bash
g++ -std=c++11 -o system src/main.cpp
```

### Bước 3: Khởi chạy
```bash
# Trên Windows
system.exe

# Trên Linux/macOS
./system
```

---

## 8. Logic nghiệp vụ

Hệ thống tuân thủ nghiêm ngặt Quy chế Đào tạo của Đại học Bách Khoa Hà Nội.

### 8.1 Thang quy đổi điểm (Utils::toGPA4 & Utils::toLetter)

| Điểm hệ 10 | Điểm hệ 4 | Điểm chữ | Xếp loại tương ứng |
|   :---:    |:---:|:---:|      :---:     |
| 9.5 – 10.0 | 4.0 |  A+ |    Xuất sắc    |
| 8.5 – 9.4  | 4.0 |  A  |    Xuất sắc    |
| 8.0 – 8.4  | 3.5 |  B+ |      Giỏi      |
| 7.0 – 7.9  | 3.0 |  B  |      Giỏi      |
| 6.5 – 6.9  | 2.5 |  C+ |       Khá      |
| 5.5 – 6.4  | 2.0 |  C  |   Trung bình   |
| 5.0 – 5.4  | 1.5 |  D+ | Trung bình yếu |
| 4.0 – 4.9  | 1.0 |  D  |       Yếu      |
|   < 4.0    | 0.0 |  F  |       Kém      |

### 8.2 Cơ chế nội suy điểm Lớp học phần
Điểm hệ 10 tự động tính toán ngay khi Giảng viên nhập điểm thành phần:
> `Điểm H10 = (CC × TS_CC) + (TC × TS_TC) + (GK × TS_GK) + (CK × TS_CK)`

### 8.3 Tính điểm trung bình tích lũy (CPA)
- Hệ thống tự động lọc lấy **điểm cao nhất** đối với các môn học được đăng ký học lại/học cải thiện.
- Môn học đã bị vô hiệu hóa (ngừng giảng dạy) vẫn được bảo toàn để tính CPA lịch sử.

---

## 9. Lưu ý & Giới hạn

- **Hiển thị tiếng Việt:** Đối với môi trường Windows, nếu console bị lỗi font chữ, hệ thống đã được tích hợp sẵn hàm đổi Code Page: `SetConsoleOutputCP(CP_UTF8);` trong `main.cpp`. Khuyến nghị sử dụng các terminal hiện đại như Windows Terminal, PowerShell 7 hoặc VSCode Terminal.
- **Luồng quản lý:** Để nhập điểm thành công, bắt buộc người dùng phải tuân thủ quy trình nghiệp vụ: 
  > Tạo Sinh viên → Tạo Môn học → Mở Lớp học phần → Thêm Sinh viên vào Lớp → Tiến hành nhập điểm.
