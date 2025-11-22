# Library Management Project

## Project Overview

This project is a simple **Library Management System** written in C++ with SQLite integration. It manages books, employees, and students, providing registration, login, and basic CRUD operations.

---

## Folder Structure

```

my_project/
├── **pycache**/
│   └── email.cpython-311.pyc
├── db/
│   ├── data_base.cpp
│   └── library.db
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

```

**Notes:**

- `__pycache__/` contains compiled Python files.
- `db/` contains database-related source code and the SQLite database.
- `src/` contains all the main C++ source and header files.
- `sqlite3.c`/`sqlite3.h` are included for database integration.

---

## Dependencies

- **C++ compiler** supporting C++11 or later (e.g., `g++`)
- **SQLite3** library (already included as `sqlite3.c` and `sqlite3.h`)
- Optional: Python 3 for `email.py` (if using email functionality)

---

## Compilation

From the `src` directory, run the following command to compile all C++ files into an executable:

```bash
cd my_project/src
g++ main.cpp menuManagement.cpp studentScreens.cpp book.cpp muon_sach.cpp nhanvien.cpp sinhvien.cpp sqlite3.c -I . -o main.exe
```

**Explanation:**

- `-I .` tells the compiler to look in the current directory for header files.
- `-o main.exe` specifies the output executable name.

---

## Running the Program

After compilation, run the executable:

```bash
./main.exe    # On Linux/Mac
main.exe      # On Windows
```

The program will guide you through:

1. Choosing role (Student or Employee)
2. Registration or login
3. Accessing the corresponding menu

---

## Contributing

If you want to contribute:

1. Fork the repository.
2. Create a new branch:

   ```bash
   git checkout -b feature/your-feature-name
   ```

3. Make changes and commit:

   ```bash
   git add .
   git commit -m "Add feature description"
   ```

4. Push your branch:

   ```bash
   git push origin feature/your-feature-name
   ```

5. Open a Pull Request.

---

## License

Specify your license here (e.g., MIT, GPL, etc.).
