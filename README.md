# Student Management System

A console-based Student Management System written in **C++** that efficiently manages student records using **file handling** and **menu-driven operations**.

---

## Features

- **Add Student** — Auto-generates a unique ID; accepts name, age, course, and GPA with input validation.
- **Display All Students** — Shows all records in a formatted table with color-coded GPA indicators.
- **Search Student** — Case-insensitive search by student name (partial match) or exact ID.
- **Update Student** — Edit any field of an existing record; leave blank to keep current value.
- **Delete Student** — Removes a record with confirmation prompt before deletion.
- **Persistent File Storage** — All data is saved to `students.dat` using `fstream` so records survive across sessions.

---

## Project Structure

```
student-management-system/
│
├── student_management.cpp   # Main source file (all logic)
├── students.dat             # Auto-generated data file (created on first run)
├── .gitignore               # Ignores compiled binary and data file
└── README.md                # Project documentation
```

---

## How to Compile & Run

### Prerequisites
- A C++ compiler supporting **C++17** (g++, clang++, MSVC)

### Linux / macOS
```bash
g++ -std=c++17 -o sms student_management.cpp
./sms
```

### Windows (MinGW)
```bash
g++ -std=c++17 -o sms.exe student_management.cpp
sms.exe
```

---

## Usage

On launch you will see the main menu:

```
============================================================
       STUDENT MANAGEMENT SYSTEM
============================================================

  [1]  Add Student
  [2]  Display All Students
  [3]  Search Student
  [4]  Update Student
  [5]  Delete Student
  [0]  Exit
```

Enter the number corresponding to the desired operation and follow the on-screen prompts.

---

## Data Storage Format

Student records are stored in `students.dat` as pipe-delimited text:

```
1|Alice Johnson|20|Computer Science|3.80
2|Bob Smith|22|Mathematics|3.20
```

Fields: `ID | Name | Age | Course | GPA`

---

## Input Validation

| Field  | Rule                        |
|--------|-----------------------------|
| Name   | Cannot be empty             |
| Age    | Must be between 5 and 120   |
| Course | Cannot be empty             |
| GPA    | Must be between 0.0 and 4.0 |

---

## Technologies Used

- **Language:** C++ (C++17)
- **File Handling:** `fstream` (ifstream / ofstream)
- **STL:** `vector`, `string`, `algorithm`, `sstream`, `iomanip`
- **Console UI:** ANSI escape codes for colored output

---

## Author

Developed as a console-based C++ project demonstrating file handling and menu-driven operations.
