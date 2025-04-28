#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Event {
protected:
    string name;
    string date;
    string venue;
public:
    Event(const string &n, const string &d, const string &v)
        : name(n), date(d), venue(v) {}
    virtual ~Event() {};
    string getName() const { return name; };
    string getDate() const { return date; };
    string getVenue() const { return venue; };
};

class Participant {
protected:
    string firstName;
    string lastName;
    string email;
public:
    Participant(const string &fn, const string &ln, const string &em)
        : firstName(fn), lastName(ln), email(em) {}
    virtual ~Participant() {};
    string getFullName() const { return firstName + " " + lastName; };
    string getEmail() const { return email; };
};

class Ticket {
protected:
    unsigned long ticketNumber;
    double price;
public:
    Ticket(unsigned long tn, double pr)
        : ticketNumber(tn), price(pr) {}
    virtual ~Ticket() {};
    unsigned long getTicketNumber() const { return ticketNumber; };
    double getPrice() const { return price; };
};

class Venue {
protected:
    string location;
    size_t capacity;
public:
    Venue(const string &loc, size_t cap)
        : location(loc), capacity(cap) {}
    virtual ~Venue() {};
    string getLocation() const { return location; };
    size_t getCapacity() const { return capacity; };
};

class Schedule {
protected:
    string startTime;
    string endTime;
public:
    Schedule(const string &st, const string &et)
        : startTime(st), endTime(et) {}
    virtual ~Schedule() {};
    string getStartTime() const { return startTime; };
    string getEndTime() const { return endTime; };
};

class Feedback {
protected:
    string participantName;
    string comment;
public:
    Feedback(const string &pn, const string &cmt)
        : participantName(pn), comment(cmt) {}
    virtual ~Feedback() {};
    string getComment() const { return comment; };
};

int main() {
    // Создание события
    Event event("Конференция ITPro", "2025-10-15", "Москва, Концерт-холл");

    // Регистрация участников
    Participant alice("Alice", "Smith", "alice.smith@mail.ru");
    Participant bob("Bob", "Johnson", "bob.johnson@gmail.com");

    // Продажа билетов
    Ticket ticketA(123456789, 500.0);
    Ticket ticketB(987654321, 600.0);

    // Место проведения
    Venue venue("Москва, Концерт-холл", 1000);

    // Расписание
    Schedule schedule("10:00 AM", "06:00 PM");

    // Сбор отзывов
    Feedback feedbackA(alice.getFullName(), "Замечательное мероприятие!");
    Feedback feedbackB(bob.getFullName(), "Всё прошло отлично.");

    // Отображение информации
    cout << "Мероприятие: " << event.getName() << ", Дата: " << event.getDate() << "\n";
    cout << "Место проведения: " << venue.getLocation() << ", Вместимость: " << venue.getCapacity() << "\n";
    cout << "Начало: " << schedule.getStartTime() << ", Окончание: " << schedule.getEndTime() << "\n\n";

    cout << "Участник #1: " << alice.getFullName() << ", Email: " << alice.getEmail() << "\n";
    cout << "Билет №" << ticketA.getTicketNumber() << ", Цена: $" << ticketA.getPrice() << "\n";
    cout << "Отзыв: " << feedbackA.getComment() << "\n\n";

    cout << "Участник #2: " << bob.getFullName() << ", Email: " << bob.getEmail() << "\n";
    cout << "Билет №" << ticketB.getTicketNumber() << ", Цена: $" << ticketB.getPrice() << "\n";
    cout << "Отзыв: " << feedbackB.getComment() << "\n";

    return 0;
}