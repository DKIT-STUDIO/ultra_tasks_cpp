#include <iostream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

class User {
private:
    string id;
    string name;
    string email;
    int borrowedCount;
public:
    User(const string& id, const string& name, const string& email)
        : id(id), name(name), email(email), borrowedCount(0) {}
    
    void incrementBorrowed() { borrowedCount++; }
    void decrementBorrowed() { borrowedCount--; }
    
    string getId() const { return id; }
    string getName() const { return name; }
    int getBorrowedCount() const { return borrowedCount; }
};

class Book {
private:
    string isbn;
    string title;
    string author;
    int publicationYear;
    bool isAvailable;
public:
    Book(const string& isbn, const string& title, const string& author, int year)
        : isbn(isbn), title(title), author(author), publicationYear(year), isAvailable(true) {}
    
    void setAvailable(bool available) { isAvailable = available; }
    
    string getIsbn() const { return isbn; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool getIsAvailable() const { return isAvailable; }
};

class Loan {
private:
    string id;
    Book* book;
    User* user;
    time_t loanDate;
    time_t dueDate;
    time_t returnDate;
    bool isReturned;
public:
    Loan(const string& id, Book* book, User* user, time_t loanDate, int loanDurationDays)
        : id(id), book(book), user(user), loanDate(loanDate), isReturned(false) {
        dueDate = loanDate + (loanDurationDays * 24 * 60 * 60);
        returnDate = 0;
    }
    
    void returnBook() {
        isReturned = true;
        returnDate = time(nullptr);
        book->setAvailable(true);
        user->decrementBorrowed();
    }
    
    bool isOverdue() const {
        if (isReturned) return false;
        return time(nullptr) > dueDate;
    }
    
    int getDaysOverdue() const {
        if (!isOverdue()) return 0;
        return (time(nullptr) - dueDate) / (24 * 60 * 60);
    }
    
    Book* getBook() const { return book; }
    User* getUser() const { return user; }
};

class Reservation {
private:
    string id;
    Book* book;
    User* user;
    time_t reservationDate;
public:
    Reservation(const string& id, Book* book, User* user)
        : id(id), book(book), user(user) {
        reservationDate = time(nullptr);
    }
    
    Book* getBook() const { return book; }
    User* getUser() const { return user; }
};

class Fine {
private:
    string id;
    User* user;
    double amount;
    string reason;
    bool isPaid;
public:
    Fine(const string& id, User* user, double amount, const string& reason)
        : id(id), user(user), amount(amount), reason(reason), isPaid(false) {}
    
    void pay() {
        isPaid = true;
    }
    
    double getAmount() const { return amount; }
    bool getIsPaid() const { return isPaid; }
};

class Library {
private:
    vector<Book> books;
    vector<User> users;
    vector<Loan> loans;
    vector<Reservation> reservations;
    vector<Fine> fines;
public:
    void addBook(const Book& book) {
        books.push_back(book);
    }
    
    void addUser(const User& user) {
        users.push_back(user);
    }
    
    bool borrowBook(User* user, Book* book, int loanDurationDays) {
        if (!book->getIsAvailable() || user->getBorrowedCount() >= 5) {
            return false;
        }
        
        string loanId = "LOAN" + to_string(loans.size() + 1);
        time_t now = time(nullptr);
        loans.emplace_back(loanId, book, user, now, loanDurationDays);
        
        book->setAvailable(false);
        user->incrementBorrowed();
        return true;
    }
    
    bool returnBook(Book* book) {
        for (auto& loan : loans) {
            if (loan.getBook() == book && !loan.isOverdue()) {
                loan.returnBook();
                return true;
            }
        }
        return false;
    }
    
    void reserveBook(User* user, Book* book) {
        string resId = "RES" + to_string(reservations.size() + 1);
        reservations.emplace_back(resId, book, user);
    }
    
    void checkOverdueLoans() {
        for (auto& loan : loans) {
            if (loan.isOverdue() && !loan.isOverdue()) {
                string fineId = "FINE" + to_string(fines.size() + 1);
                int daysOverdue = loan.getDaysOverdue();
                double amount = daysOverdue * 0.50; // $0.50 per day
                fines.emplace_back(fineId, loan.getUser(), amount, "Late return");
            }
        }
    }
    
    vector<Book> searchByTitle(const string& title) const {
        vector<Book> result;
        for (const auto& book : books) {
            if (book.getTitle().find(title) != string::npos) {
                result.push_back(book);
            }
        }
        return result;
    }
    
    vector<Book> searchByAuthor(const string& author) const {
        vector<Book> result;
        for (const auto& book : books) {
            if (book.getAuthor().find(author) != string::npos) {
                result.push_back(book);
            }
        }
        return result;
    }
};

int main() {
    Library library;
    
    // Добавляем книги
    Book book1("978-3-16-148410-0", "The Great Novel", "Author One", 2001);
    Book book2("978-1-23-456789-7", "Programming Basics", "Tech Writer", 2015);
    library.addBook(book1);
    library.addBook(book2);
    
    // Добавляем пользователей
    User user1("U1001", "Alice", "alice@example.com");
    User user2("U1002", "Bob", "bob@example.com");
    library.addUser(user1);
    library.addUser(user2);
    
    // Пользователь берет книгу
    library.borrowBook(&user1, &book1, 14);
    
    // Поиск книг
    auto results = library.searchByTitle("Programming");
    for (const auto& book : results) {
        cout << "Found: " << book.getTitle() << " by " << book.getAuthor() << endl;
    }
    
    return 0;
}