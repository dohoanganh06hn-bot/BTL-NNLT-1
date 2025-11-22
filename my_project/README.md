Folder structure of the project:

my_project/
├── **pycache**/
│ └── email.cpython-311.pyc
├── db/
│ ├── data_base.cpp
│ └── library.db
└── src/
├── book.cpp
├── book.h
├── email.py
├── main.cpp
├── menuManagement.cpp
├── menuManagement.h
├── muon_sach.cpp
├── muon_sach.h
├── nhanvien.cpp
├── nhanvien.h
├── sinhvien.cpp
├── sinhvien.h
├── sqlite3.c
├── sqlite3.h
├── studentScreens.cpp
└── studentScreens.h

Compile all the cpp files command:
cd my_project/src && g++ main.cpp menuManagement.cpp studentScreens.cpp book.cpp muon_sach.cpp nhanvien.cpp sinhvien.cpp sqlite3.c -I . -o main.exe
