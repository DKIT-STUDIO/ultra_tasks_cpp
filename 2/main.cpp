#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

class Student {
private:
    string id;
    string name;
    string email;
public:
    Student(const string& id, const string& name, const string& email)
        : id(id), name(name), email(email) {}
    
    string getId() const { return id; }
    string getName() const { return name; }
};

class Instructor {
private:
    string id;
    string name;
    string department;
public:
    Instructor(const string& id, const string& name, const string& department)
        : id(id), name(name), department(department) {}
    
    string getId() const { return id; }
    string getName() const { return name; }
};

class Course {
private:
    string code;
    string title;
    int credits;
    Instructor* instructor;
public:
    Course(const string& code, const string& title, int credits)
        : code(code), title(title), credits(credits), instructor(nullptr) {}
    
    void assignInstructor(Instructor* instr) {
        instructor = instr;
    }
    
    string getCode() const { return code; }
    string getTitle() const { return title; }
    Instructor* getInstructor() const { return instructor; }
};

class Enrollment {
private:
    Student* student;
    Course* course;
    string semester;
    char grade;
public:
    Enrollment(Student* student, Course* course, const string& semester)
        : student(student), course(course), semester(semester), grade('N') {}
    
    void assignGrade(char grade) {
        this->grade = grade;
    }
    
    Student* getStudent() const { return student; }
    Course* getCourse() const { return course; }
    char getGrade() const { return grade; }
};

class Schedule {
private:
    Course* course;
    string day;
    string time;
    string room;
public:
    Schedule(Course* course, const string& day, const string& time, const string& room)
        : course(course), day(day), time(time), room(room) {}
    
    Course* getCourse() const { return course; }
    string getDayTime() const { return day + " " + time; }
};

class GradeReport {
private:
    Student* student;
    vector<Enrollment*> enrollments;
public:
    GradeReport(Student* student) : student(student) {}
    
    void addEnrollment(Enrollment* enrollment) {
        enrollments.push_back(enrollment);
    }
    
    void generateReport() const {
        cout << "Grade Report for " << student->getName() << " (ID: " << student->getId() << ")\n";
        cout << "----------------------------------------\n";
        for (const auto& enroll : enrollments) {
            cout << enroll->getCourse()->getCode() << " - " << enroll->getCourse()->getTitle() 
                 << ": " << enroll->getGrade() << endl;
        }
    }
};

int main() {
    // Создаем студентов
    Student student1("S1001", "Alice Johnson", "alice@university.edu");
    Student student2("S1002", "Bob Smith", "bob@university.edu");
    
    // Создаем преподавателей
    Instructor instructor1("I2001", "Dr. Smith", "Computer Science");
    Instructor instructor2("I2002", "Prof. Brown", "Mathematics");
    
    // Создаем курсы
    Course course1("CS101", "Introduction to Programming", 4);
    course1.assignInstructor(&instructor1);
    
    Course course2("MATH201", "Calculus II", 3);
    course2.assignInstructor(&instructor2);
    
    // Регистрируем студентов на курсы
    Enrollment enroll1(&student1, &course1, "Fall 2023");
    enroll1.assignGrade('A');
    
    Enrollment enroll2(&student1, &course2, "Fall 2023");
    enroll2.assignGrade('B');
    
    Enrollment enroll3(&student2, &course1, "Fall 2023");
    enroll3.assignGrade('B');
    
    // Создаем расписание
    Schedule schedule1(&course1, "Monday/Wednesday", "10:00-11:30", "Building A, Room 101");
    Schedule schedule2(&course2, "Tuesday/Thursday", "13:00-14:30", "Building B, Room 205");
    
    // Генерируем отчет об успеваемости
    GradeReport report(&student1);
    report.addEnrollment(&enroll1);
    report.addEnrollment(&enroll2);
    report.generateReport();
    
    return 0;
}