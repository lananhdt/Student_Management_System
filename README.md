# Hệ Thống Quản Lý Sinh Viên & Điểm Số
### Student Management System – Console Application

---

## Mục lục

1. [Mô tả tổng quan](#1-mô-tả-tổng-quan)
2. [Tính năng](#2-tính-năng)
3. [Yêu cầu hệ thống](#3-yêu-cầu-hệ-thống)
4. [Cấu trúc dự án](#4-cấu-trúc-dự-án)
5. [Cấu trúc dữ liệu & Thuật toán](#5-cấu-trúc-dữ-liệu--thuật-toán)
6. [Định dạng file dữ liệu](#6-định-dạng-file-dữ-liệu)
7. [Hướng dẫn cài đặt & Biên dịch](#7-hướng-dẫn-cài-đặt--biên-dịch)
8. [Hướng dẫn sử dụng](#8-hướng-dẫn-sử-dụng)
9. [Sơ đồ menu](#9-sơ-đồ-menu)
10. [Logic nghiệp vụ](#10-logic-nghiệp-vụ)
11. [Ví dụ Input / Output](#11-ví-dụ-input--output)
12. [Lưu ý & Giới hạn](#12-lưu-ý--giới-hạn)

---

## 1. Mô tả tổng quan

Chương trình quản lý sinh viên và kết quả học tập viết bằng **C++ thuần (chuẩn C++11)**, chạy hoàn toàn trên **terminal console**. Toàn bộ cấu trúc dữ liệu và thuật toán được **tự cài đặt từ đầu** — không sử dụng bất kỳ container (`std::list`, `std::map`, `std::unordered_map`, `std::sort`…) hay thư viện ngoài nào.

Dữ liệu được lưu trữ bền vững trong **3 file text thuần** và tự động tải lên khi khởi động, lưu xuống khi thực hiện mỗi thao tác ghi.

---

## 2. Tính năng

### 2.1 Quản lý Sinh viên
| # | Tính năng |
|---|-----------|
| ✅ | Thêm sinh viên mới (MSSV, họ tên, lớp, ngày sinh, ngành, email, trạng thái) |
| ✅ | Sửa thông tin sinh viên (giữ nguyên MSSV) |
| ✅ | Xóa sinh viên (có xác nhận) |
| ✅ | Xem danh sách toàn bộ sinh viên |
| ✅ | Tìm kiếm theo MSSV — O(1) qua Hash Map |
| ✅ | Tìm kiếm theo họ tên (chứa từ khóa) |
| ✅ | Sắp xếp danh sách theo MSSV tăng dần |
| ✅ | Sắp xếp danh sách theo họ tên tăng dần |

### 2.2 Quản lý Môn học
| # | Tính năng |
|---|-----------|
| ✅ | Thêm môn học (mã môn, tên môn, số tín chỉ) |
| ✅ | Sửa thông tin môn học |
| ✅ | Xóa môn học |
| ✅ | Xem danh sách môn học |
| ✅ | Tìm kiếm môn học theo mã môn — O(1) |

### 2.3 Quản lý Điểm số
| # | Tính năng |
|---|-----------|
| ✅ | Nhập điểm theo MSSV + mã môn học (hệ 10) |
| ✅ | Tự động tính điểm hệ 4 và ký hiệu chữ (A, B+, B, …) |
| ✅ | Cập nhật điểm (ghi đè nếu đã tồn tại) |
| ✅ | Xóa bản ghi điểm |
| ✅ | Xem toàn bộ bản ghi điểm |

### 2.4 Báo cáo & Tìm kiếm
| # | Tính năng |
|---|-----------|
| ✅ | Bảng điểm chi tiết của 1 sinh viên (kèm GPA tích lũy + xếp loại) |
| ✅ | Bảng điểm lớp học phần theo mã môn (sắp xếp điểm giảm dần, thống kê phân bố) |
| ✅ | Bảng xếp hạng toàn trường theo GPA tích lũy (giảm dần) |

---

## 3. Yêu cầu hệ thống

### Phần cứng (tối thiểu)
| Thành phần | Yêu cầu |
|---|---|
| CPU | Bất kỳ, x86 / x86-64 / ARM |
| RAM | 16 MB trở lên (chương trình rất nhẹ) |
| Ổ đĩa | 5 MB trống (cho binary + data files) |
| Màn hình | Terminal hỗ trợ UTF-8, rộng ≥ 80 cột |

### Phần mềm
| Thành phần | Yêu cầu |
|---|---|
| Hệ điều hành | Windows / Linux / macOS |
| Trình biên dịch | `g++` (GCC ≥ 5.0) hoặc `clang++` ≥ 3.5, hỗ trợ C++11 |
| Thư viện | Chỉ C++ Standard Library (`<iostream>`, `<fstream>`, `<string>`, `<iomanip>`, `<sstream>`) |

> **Không cần** cài thêm bất kỳ thư viện bên ngoài nào.

---

## 4. Cấu trúc dự án

```
student_management_system/
│
├── main.cpp              # Entry point: load → menu → save
│
├── Models.h              # Tất cả struct dữ liệu và node
├── Utils.h               # Hàm tiện ích: chuyển điểm, nhập liệu, hiển thị
├── HashMap.h             # Hash Map tự cài (StudentHashMap + SubjectHashMap)
├── StudentManager.h      # Quản lý sinh viên: DLL + MergeSort + CRUD
├── SubjectManager.h      # Quản lý môn học: DLL + CRUD
├── ScoreManager.h        # Quản lý điểm: DLL + tính GPA + báo cáo
├── FileManager.h         # Đọc/ghi 3 file text
├── Menu.h                # Vòng lặp menu console 4 tầng
│
├── students.txt          # [Tự sinh] Dữ liệu sinh viên
├── subjects.txt          # [Tự sinh] Dữ liệu môn học
└── scores.txt            # [Tự sinh] Dữ liệu điểm số
```

**Tổng số dòng code:** ~1.432 dòng (không tính file data)

### Sơ đồ phụ thuộc giữa các file

```
main.cpp
  └── Menu.h
        ├── StudentManager.h
        │     ├── Models.h
        │     ├── HashMap.h
        │     └── Utils.h
        ├── SubjectManager.h
        │     ├── Models.h
        │     ├── HashMap.h
        │     └── Utils.h
        ├── ScoreManager.h
        │     ├── StudentManager.h
        │     ├── SubjectManager.h
        │     └── Utils.h
        └── FileManager.h
              ├── StudentManager.h
              ├── SubjectManager.h
              └── ScoreManager.h
```

---

## 5. Cấu trúc dữ liệu & Thuật toán

### 5.1 Doubly Linked List (Danh sách liên kết đôi)
Tự cài đặt với 3 loại node riêng biệt: `SNode` (sinh viên), `SubNode` (môn học), `ScNode` (điểm). Mỗi node giữ con trỏ `prev` và `next`.

```
NULL ← [SV001|An] ⟺ [SV002|Binh] ⟺ [SV003|Cuong] → NULL
         head                              tail
```

**Ưu điểm:** Chèn/xóa O(1) khi có con trỏ node, duyệt hai chiều.

### 5.2 Hash Map (Bảng băm — Separate Chaining)
- `StudentHashMap`: capacity = 251 bucket (số nguyên tố), key = MSSV → `SNode*`
- `SubjectHashMap`: capacity = 127 bucket, key = mã môn → `SubNode*`
- Hàm băm: `h = (h * 31 + c) % capacity` (Polynomial Rolling Hash)
- Giải quyết đụng độ: Chaining bằng linked list trên mỗi bucket

**Tra cứu sinh viên / môn học: O(1) trung bình**

```
hash("SV001") = 47
table[47] → [SV001 → node*] → [SV099 → node*] → NULL
```

### 5.3 Merge Sort (Sắp xếp trộn)
Tự cài đặt trên Doubly Linked List, đệ quy, **O(n log n)**.

Hỗ trợ 2 tiêu chí cho sinh viên:
- `sortById()` — sắp xếp tăng dần theo MSSV (so sánh chuỗi lexicographic)
- `sortByName()` — sắp xếp tăng dần theo họ tên

Thuật toán:
```
MergeSort(head):
  1. Tìm node giữa bằng slow/fast pointer
  2. Tách list thành 2 nửa
  3. Đệ quy sort mỗi nửa
  4. Merge 2 nửa đã sort
  5. Rebuild prev links và cập nhật tail
```

### 5.4 Tính GPA tích lũy
Áp dụng công thức **GPA có trọng số tín chỉ**:

```
GPA_tích_lũy = Σ(điểm_hệ_4 × tín_chỉ_môn) / Σ(tín_chỉ_môn)
```

---

## 6. Định dạng file dữ liệu

Các file dữ liệu được tự động tạo ra khi lưu lần đầu. Separator là ký tự `|` (pipe) để tránh xung đột với dấu phẩy trong tên.

### `students.txt`
```
MSSV|Ho_va_ten|Lop|Ngay_sinh|Nganh|Email|Trang_thai
```
**Ví dụ:**
```
SV001|Nguyen Van An|CNPM-K65|01/01/2002|CNTT|an@edu.vn|Dang hoc
SV002|Tran Thi Binh|KHMT-K65|15/05/2001|KHMT|binh@edu.vn|Dang hoc
SV003|Le Hoang Cuong|ATTT-K64|20/03/2000|ATTT|cuong@edu.vn|Canh bao
```

**Trạng thái hợp lệ:** `Dang hoc` | `Bao luu` | `Canh bao` | `Tot nghiep`

### `subjects.txt`
```
Ma_mon|Ten_mon_hoc|So_tin_chi
```
**Ví dụ:**
```
IT3040|Lap trinh huong doi tuong|3
IT2020|Co so du lieu|3
IT1010|Giai tich 1|2
```

### `scores.txt`
```
MSSV|Ma_mon|Diem_he_10
```
**Ví dụ:**
```
SV001|IT3040|8.5
SV001|IT2020|7.0
SV002|IT3040|9.0
```
> Điểm hệ 4 và ký hiệu chữ **không lưu vào file** — tính lại mỗi lần load để tránh dư thừa dữ liệu.

---

## 7. Hướng dẫn cài đặt & Biên dịch

### Bước 1: Clone / tải source code
```bash
git clone https://github.com/your-repo/student_management_system.git
cd student_management_system
```

### Bước 2: Biên dịch

**Linux / macOS:**
```bash
g++ -std=c++11 -o sms main.cpp
```

**Windows (MinGW / MSYS2):**
```bash
g++ -std=c++11 -o sms.exe main.cpp
```

**Biên dịch với cảnh báo đầy đủ (khuyến nghị khi phát triển):**
```bash
g++ -std=c++11 -Wall -Wextra -o sms main.cpp
```

### Bước 3: Chạy chương trình
```bash
./sms          # Linux / macOS
sms.exe        # Windows
```

> **Lưu ý:** Chạy chương trình từ thư mục chứa file `.cpp` để các file `students.txt`, `subjects.txt`, `scores.txt` được tạo đúng vị trí.

### Bước 4 (tuỳ chọn): Chuẩn bị dữ liệu mẫu
Nếu muốn chạy với dữ liệu có sẵn, tạo 3 file text trong cùng thư mục với binary theo định dạng ở mục 6.

---

## 8. Hướng dẫn sử dụng

### Quy tắc nhập liệu chung

| Tình huống | Hành động |
|---|---|
| Nhập số nguyên | Gõ số rồi nhấn **Enter** |
| Nhập chuỗi (tên, mã,...) | Gõ nội dung rồi nhấn **Enter** |
| Nhập điểm | Số thực trong `[0.0, 10.0]`, VD: `8.5` |
| Bỏ trống khi sửa | Nhấn **Enter** ngay để giữ giá trị cũ |
| Xác nhận xóa | Gõ `y` hoặc `Y` để xác nhận, bất kỳ ký tự khác để hủy |
| Quay lại | Chọn `[0]` ở bất kỳ menu nào |

### Quy trình nhập liệu cơ bản

**Bước khởi đầu được khuyến nghị:**
1. Vào **Menu 2** → Thêm các môn học
2. Vào **Menu 1** → Thêm sinh viên
3. Vào **Menu 3** → Nhập điểm theo từng cặp MSSV + mã môn
4. Vào **Menu 4** → Xem báo cáo

---

## 9. Sơ đồ menu

```
[MENU CHÍNH]
├── [1] Quản lý Sinh viên
│     ├── [1] Xem danh sách sinh viên
│     ├── [2] Thêm sinh viên mới
│     ├── [3] Sửa thông tin sinh viên
│     ├── [4] Xóa sinh viên
│     ├── [5] Tìm kiếm theo MSSV
│     ├── [6] Tìm kiếm theo họ tên
│     ├── [7] Sắp xếp theo MSSV (Merge Sort)
│     ├── [8] Sắp xếp theo họ tên (Merge Sort)
│     └── [0] Quay lại
│
├── [2] Quản lý Môn học
│     ├── [1] Xem danh sách môn học
│     ├── [2] Thêm môn học mới
│     ├── [3] Sửa thông tin môn học
│     ├── [4] Xóa môn học
│     ├── [5] Tìm kiếm theo mã môn
│     └── [0] Quay lại
│
├── [3] Quản lý Điểm số
│     ├── [1] Nhập / Cập nhật điểm
│     ├── [2] Xóa điểm
│     ├── [3] Xem toàn bộ bản ghi điểm
│     └── [0] Quay lại
│
├── [4] Báo cáo & Tìm kiếm
│     ├── [1] Bảng điểm của 1 sinh viên
│     ├── [2] Bảng điểm lớp học phần
│     ├── [3] Bảng xếp hạng GPA tích lũy
│     └── [0] Quay lại
│
└── [0] Thoát (lưu tự động)
```

---

## 10. Logic nghiệp vụ

### 10.1 Bảng chuyển đổi điểm

| Điểm hệ 10 | Điểm hệ 4 | Ký hiệu chữ |
|:-----------:|:---------:|:-----------:|
| 9.0 – 10.0  | 4.0       | A           |
| 8.5 – 8.9   | 3.7       | A-          |
| 8.0 – 8.4   | 3.5       | B+          |
| 7.5 – 7.9   | 3.0       | B           |
| 7.0 – 7.4   | 2.5       | B-          |
| 6.5 – 6.9   | 2.3       | C+          |
| 6.0 – 6.4   | 2.0       | C           |
| 5.5 – 5.9   | 1.7       | C-          |
| 5.0 – 5.4   | 1.3       | D+          |
| 4.0 – 4.9   | 1.0       | D           |
| 0.0 – 3.9   | 0.0       | F           |

*(Theo thang điểm Bộ Giáo dục & Đào tạo Việt Nam)*

### 10.2 Xếp loại học lực theo GPA tích lũy

| GPA tích lũy | Xếp loại   |
|:------------:|:----------:|
| ≥ 3.60       | Xuất sắc   |
| 3.20 – 3.59  | Giỏi       |
| 2.50 – 3.19  | Khá        |
| 2.00 – 2.49  | Trung bình |
| 1.00 – 1.99  | Yếu        |
| < 1.00       | Kém        |

### 10.3 Ràng buộc dữ liệu

| Trường | Ràng buộc |
|---|---|
| MSSV | Không trùng lặp, không rỗng |
| Mã môn | Không trùng lặp, không rỗng |
| Điểm hệ 10 | Số thực trong `[0.0, 10.0]` |
| Số tín chỉ | Số nguyên dương |
| Cặp (MSSV, mã môn) | Mỗi sinh viên chỉ có 1 bản ghi điểm / môn học; nhập lại sẽ cập nhật |

---

## 11. Ví dụ Input / Output

### Ví dụ 1: Xem bảng điểm sinh viên SV001

**Input (chọn menu):**
```
> 4       ← Báo cáo & Tìm kiếm
> 1       ← Bảng điểm 1 sinh viên
> SV001   ← Nhập MSSV
```

**Output:**
```
========================================================================
                        BANG DIEM SINH VIEN
========================================================================
  MSSV    : SV001
  Ho ten  : Nguyen Van An
  Lop     : CNPM-K65
  Nganh   : CNTT
------------------------------------------------------------------------
Ma mon    Ten mon hoc                   TC   Diem 10  Diem 4  Chu
------------------------------------------------------------------------
IT3040    Lap trinh huong doi tuong      3     8.5     3.7   A
IT2020    Co so du lieu                  3     7.0     2.5   B
IT1010    Giai tich 1                    2     9.0     4.0   A
------------------------------------------------------------------------
  Tong tin chi tich luy : 8
  GPA tich luy (he 4)   : 3.33
  Xep loai hoc luc       : Gioi
------------------------------------------------------------------------
```

### Ví dụ 2: Bảng xếp hạng GPA toàn trường

**Input:**
```
> 4       ← Báo cáo & Tìm kiếm
> 3       ← Bảng xếp hạng GPA
```

**Output:**
```
========================================================================
           BANG XEP HANG SINH VIEN THEO GPA TICH LUY
========================================================================
TT   MSSV         Ho va ten                 Lop         TC   GPA    Xep loai
------------------------------------------------------------------------
  1.  SV001        Nguyen Van An             CNPM-K65     8   3.33  Gioi
  2.  SV002        Tran Thi Binh             KHMT-K65     8   3.24  Gioi
  3.  SV003        Le Hoang Cuong            ATTT-K64     6   1.15  Yeu
------------------------------------------------------------------------
```

### Ví dụ 3: Bảng điểm lớp học phần IT3040

**Input:**
```
> 4         ← Báo cáo & Tìm kiếm
> 2         ← Bảng điểm lớp học phần
> IT3040    ← Mã môn
```

**Output:**
```
========================================================================
                     BANG DIEM LOP HOC PHAN
========================================================================
  Ma mon  : IT3040
  Ten mon : Lap trinh huong doi tuong
  Tin chi : 3
------------------------------------------------------------------------
MSSV         Ho va ten                 Lop         Diem 10  Diem 4  Chu
------------------------------------------------------------------------
SV002        Tran Thi Binh             KHMT-K65       9.0    4.0   A
SV001        Nguyen Van An             CNPM-K65       8.5    3.7   A
SV003        Le Hoang Cuong            ATTT-K64       4.5    1.0   D+
------------------------------------------------------------------------
  So sinh vien : 3
  Diem TB lop  : 7.33
  Phan bo xep loai: A=2  B=0  C=0  D=1  F=0
------------------------------------------------------------------------
```

---

## 12. Lưu ý & Giới hạn

### Lưu ý khi sử dụng
- **Encoding:** File dữ liệu dùng UTF-8. Nếu terminal Windows không hiển thị đúng tiếng Việt, chạy lệnh `chcp 65001` trước khi khởi động chương trình.
- **Xóa sinh viên:** Bản ghi điểm của sinh viên đó **không tự động xóa theo**. Cần xóa thủ công trong Menu 3 nếu cần dọn dẹp dữ liệu.
- **Xóa môn học:** Tương tự — bản ghi điểm của môn đó cần xóa riêng.
- **Sửa dữ liệu file trực tiếp:** Được phép, nhưng phải đảm bảo đúng định dạng `|` separator và đủ số trường. Dòng lỗi định dạng sẽ tự động bị bỏ qua khi load.

### Giới hạn kỹ thuật
| Giới hạn | Giá trị |
|---|---|
| Số sinh viên tối đa | ~10.000 (giới hạn RAM, không giới hạn cứng) |
| Số môn học tối đa | ~5.000 |
| Capacity Hash Map sinh viên | 251 bucket |
| Capacity Hash Map môn học | 127 bucket |
| Độ dài chuỗi tên / mã | Không giới hạn cứng (`std::string`) |
| Ký tự không được dùng trong dữ liệu | `\|` (pipe) — dùng làm separator |

### Độ phức tạp thời gian các thao tác chính

| Thao tác | Độ phức tạp |
|---|---|
| Tìm sinh viên / môn học theo mã | O(1) trung bình |
| Thêm / Xóa sinh viên | O(1) |
| Sắp xếp danh sách sinh viên | O(n log n) |
| Tính GPA tích lũy 1 sinh viên | O(m) — m là số môn đã học |
| In bảng xếp hạng n sinh viên | O(n²) insertion sort tạm |
| Load / Save file | O(n) |

---

*Dự án môn Cấu trúc Dữ liệu & Giải thuật — Ngôn ngữ C++11 — Tự cài đặt toàn bộ CTDL & thuật toán*
