#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;

// ─── ANSI color codes ──────────────────────────────────────────────────────
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define RED     "\033[31m"
#define MAGENTA "\033[35m"
#define BLUE    "\033[34m"
#define DIM     "\033[2m"

// ─── Student Structure ─────────────────────────────────────────────────────
struct Student {
    int    id;
    string name;
    int    age;
    string course;
    float  gpa;
};

// ─── File path ─────────────────────────────────────────────────────────────
const string FILE_PATH = "students.dat";

// ─── Utility helpers ───────────────────────────────────────────────────────
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pauseScreen() {
    cout << DIM << "\n  Press Enter to continue..." << RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void printLine(char c = '-', int w = 60) {
    cout << DIM;
    for (int i = 0; i < w; i++) cout << c;
    cout << RESET << "\n";
}

void printHeader(const string& title) {
    cout << "\n";
    printLine();
    cout << CYAN << BOLD << "  " << title << RESET << "\n";
    printLine();
}

// ─── File I/O ──────────────────────────────────────────────────────────────
vector<Student> loadStudents() {
    vector<Student> students;
    ifstream fin(FILE_PATH);
    if (!fin.is_open()) return students;

    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        istringstream ss(line);
        Student s;
        string token;

        getline(ss, token, '|'); s.id     = stoi(token);
        getline(ss, s.name,    '|');
        getline(ss, token,     '|'); s.age    = stoi(token);
        getline(ss, s.course,  '|');
        getline(ss, token,     '|'); s.gpa    = stof(token);

        students.push_back(s);
    }
    fin.close();
    return students;
}

void saveStudents(const vector<Student>& students) {
    ofstream fout(FILE_PATH, ios::trunc);
    for (const auto& s : students) {
        fout << s.id     << "|"
             << s.name   << "|"
             << s.age    << "|"
             << s.course << "|"
             << fixed << setprecision(2) << s.gpa
             << "\n";
    }
    fout.close();
}

// ─── Display helpers ───────────────────────────────────────────────────────
void printTableHeader() {
    cout << "\n"
         << BOLD << CYAN
         << left  << setw(6)  << "  ID"
         << setw(22) << "Name"
         << setw(6)  << "Age"
         << setw(18) << "Course"
         << setw(6)  << "GPA"
         << RESET << "\n";
    printLine();
}

void printStudent(const Student& s) {
    string gpaColor = (s.gpa >= 3.5) ? GREEN : (s.gpa >= 2.5) ? YELLOW : RED;
    cout << "  "
         << left  << setw(4)  << s.id
         << setw(22) << s.name
         << setw(6)  << s.age
         << setw(18) << s.course
         << gpaColor << fixed << setprecision(2) << s.gpa << RESET
         << "\n";
}

// ─── Operations ────────────────────────────────────────────────────────────
void addStudent() {
    printHeader("ADD NEW STUDENT");

    vector<Student> students = loadStudents();

    Student s;
    // Auto-generate ID
    s.id = 1;
    for (const auto& st : students)
        if (st.id >= s.id) s.id = st.id + 1;

    cout << GREEN << "  Auto-assigned ID: " << BOLD << s.id << RESET << "\n\n";

    cout << "  Name   : "; getline(cin, s.name);
    if (s.name.empty()) { cout << RED << "  Name cannot be empty.\n" << RESET; pauseScreen(); return; }

    cout << "  Age    : "; cin >> s.age; clearInput();
    if (s.age < 5 || s.age > 120) { cout << RED << "  Invalid age.\n" << RESET; pauseScreen(); return; }

    cout << "  Course : "; getline(cin, s.course);
    if (s.course.empty()) { cout << RED << "  Course cannot be empty.\n" << RESET; pauseScreen(); return; }

    cout << "  GPA    : "; cin >> s.gpa; clearInput();
    if (s.gpa < 0.0f || s.gpa > 4.0f) { cout << RED << "  GPA must be 0.0 – 4.0.\n" << RESET; pauseScreen(); return; }

    students.push_back(s);
    saveStudents(students);

    cout << GREEN << "\n  ✓ Student \"" << s.name << "\" added successfully!\n" << RESET;
    pauseScreen();
}

void displayAll() {
    printHeader("ALL STUDENTS");
    vector<Student> students = loadStudents();

    if (students.empty()) {
        cout << YELLOW << "  No student records found.\n" << RESET;
        pauseScreen();
        return;
    }

    printTableHeader();
    for (const auto& s : students) printStudent(s);
    printLine();
    cout << DIM << "  Total records: " << students.size() << RESET << "\n";
    pauseScreen();
}

void searchStudent() {
    printHeader("SEARCH STUDENT");
    vector<Student> students = loadStudents();

    cout << "  Enter Name or ID to search: ";
    string query;
    getline(cin, query);

    vector<Student> results;
    for (const auto& s : students) {
        string nameLower = s.name;
        string qLower    = query;
        transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
        transform(qLower.begin(),    qLower.end(),    qLower.begin(),    ::tolower);

        if (nameLower.find(qLower) != string::npos ||
            to_string(s.id) == query) {
            results.push_back(s);
        }
    }

    if (results.empty()) {
        cout << RED << "\n  No match found for \"" << query << "\".\n" << RESET;
    } else {
        printTableHeader();
        for (const auto& s : results) printStudent(s);
        printLine();
        cout << GREEN << "  Found " << results.size() << " result(s).\n" << RESET;
    }
    pauseScreen();
}

void updateStudent() {
    printHeader("UPDATE STUDENT");
    vector<Student> students = loadStudents();

    cout << "  Enter Student ID to update: ";
    int id; cin >> id; clearInput();

    for (auto& s : students) {
        if (s.id == id) {
            cout << "\n  Current record:\n";
            printTableHeader();
            printStudent(s);
            printLine();

            cout << "\n  Enter new details (leave blank to keep current):\n\n";

            cout << "  Name   [" << s.name   << "]: ";
            string tmp; getline(cin, tmp);
            if (!tmp.empty()) s.name = tmp;

            cout << "  Age    [" << s.age    << "]: ";
            getline(cin, tmp);
            if (!tmp.empty()) s.age = stoi(tmp);

            cout << "  Course [" << s.course << "]: ";
            getline(cin, tmp);
            if (!tmp.empty()) s.course = tmp;

            cout << "  GPA    [" << fixed << setprecision(2) << s.gpa << "]: ";
            getline(cin, tmp);
            if (!tmp.empty()) s.gpa = stof(tmp);

            saveStudents(students);
            cout << GREEN << "\n  ✓ Student ID " << id << " updated successfully!\n" << RESET;
            pauseScreen();
            return;
        }
    }

    cout << RED << "\n  Student with ID " << id << " not found.\n" << RESET;
    pauseScreen();
}

void deleteStudent() {
    printHeader("DELETE STUDENT");
    vector<Student> students = loadStudents();

    cout << "  Enter Student ID to delete: ";
    int id; cin >> id; clearInput();

    auto it = find_if(students.begin(), students.end(),
                      [&](const Student& s){ return s.id == id; });

    if (it == students.end()) {
        cout << RED << "\n  Student with ID " << id << " not found.\n" << RESET;
        pauseScreen();
        return;
    }

    cout << "\n  Record to delete:\n";
    printTableHeader();
    printStudent(*it);
    printLine();

    cout << YELLOW << "\n  Confirm deletion? (y/N): " << RESET;
    char ch; cin >> ch; clearInput();

    if (ch == 'y' || ch == 'Y') {
        students.erase(it);
        saveStudents(students);
        cout << GREEN << "\n  ✓ Student ID " << id << " deleted successfully!\n" << RESET;
    } else {
        cout << DIM << "\n  Deletion cancelled.\n" << RESET;
    }
    pauseScreen();
}

// ─── Main Menu ─────────────────────────────────────────────────────────────
void showMenu() {
    cout << "\n\033[2J\033[H";   // clear screen
    printLine('=');
    cout << CYAN << BOLD
         << "       STUDENT MANAGEMENT SYSTEM\n"
         << RESET;
    printLine('=');
    cout << "\n"
         << GREEN  << "  [1]" << RESET << "  Add Student\n"
         << CYAN   << "  [2]" << RESET << "  Display All Students\n"
         << YELLOW << "  [3]" << RESET << "  Search Student\n"
         << BLUE   << "  [4]" << RESET << "  Update Student\n"
         << RED    << "  [5]" << RESET << "  Delete Student\n"
         << DIM    << "  [0]  Exit\n" << RESET
         << "\n";
    printLine();
    cout << "  Choice: ";
}

int main() {
    int choice;
    do {
        showMenu();
        cin >> choice;
        clearInput();

        switch (choice) {
            case 1: addStudent();    break;
            case 2: displayAll();    break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 0:
                cout << CYAN << "\n  Goodbye!\n\n" << RESET;
                break;
            default:
                cout << RED << "\n  Invalid option. Try again.\n" << RESET;
                pauseScreen();
        }
    } while (choice != 0);

    return 0;
}
