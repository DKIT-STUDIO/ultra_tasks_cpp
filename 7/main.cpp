#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

class User {
private:
    string id;
    string name;
    string email;
public:
    User(string id, string name, string email) : id(id), name(name), email(email) {}
    string getId() const { return id; }
    string getName() const { return name; }
};

class Task {
private:
    string id;
    string title;
    string description;
    User* assignee;
    int estimatedHours;
    int actualHours;
public:
    Task(string id, string title, string desc) 
        : id(id), title(title), description(desc), assignee(nullptr), estimatedHours(0), actualHours(0) {}
    
    void assignTo(User* user) { assignee = user; }
    void logHours(int hours) { actualHours += hours; }
 string getId() const { return id; }
    int getActualHours() const { return actualHours; }
};

// Исправление класса ExternalAPI
class ExternalAPI {
private:
    string name;
    string endpoint;
    map<string, string> config;
public:
    ExternalAPI(string name, string endpoint) : name(name), endpoint(endpoint) {}
    
    // Добавлен метод getName()
    string getName() const { return name; }
    
    void setConfig(string key, string value) { config[key] = value; }
    string getData(string query) const {
        // В реальной системе здесь было бы HTTP-соединение
        return "Mock data for query: " + query;
    }
};

class Notification {
private:
    string type;
    string message;
    User* recipient;
    time_t timestamp;
public:
    Notification(string type, string message, User* recipient) 
        : type(type), message(message), recipient(recipient) {
        timestamp = time(nullptr);
    }
    
    void send() {
        // В реальной системе здесь была бы отправка email/SMS и т.д.
        cout << "Notification to " << recipient->getName() << ": " << message << endl;
    }
};

class Report {
private:
    string id;
    string title;
    map<string, string> data;
public:
    Report(string id, string title) : id(id), title(title) {}
    
    void addData(string key, string value) { data[key] = value; }
    void generate() const {
        cout << "Report: " << title << endl;
        for (const auto& item : data) {
            cout << item.first << ": " << item.second << endl;
        }
    }
};

class Project {
private:
    string id;
    string name;
    vector<Task> tasks;
    vector<User> members;
    vector<ExternalAPI> integrations;
public:
    Project(string id, string name) : id(id), name(name) {}
    
    void addMember(User user) { members.push_back(user); }
    void addTask(Task task) { tasks.push_back(task); }
    void addIntegration(ExternalAPI api) { integrations.push_back(api); }
    
    void syncWithAPI(string apiName) {
        for (auto& api : integrations) {
            if (apiName == api.getName()) {
                string data = api.getData("tasks");
                cout << "Synced data from " << apiName << ": " << data << endl;
            }
        }
    }
    
    void generateTimeReport() {
        Report report("TIME-001", "Time Tracking Report");
        int totalHours = 0;
        for (const auto& task : tasks) {
            totalHours += task.getActualHours();
        }
        report.addData("Total tasks", to_string(tasks.size()));
        report.addData("Total hours logged", to_string(totalHours));
        report.generate();
    }
};

int main() {
    // Создаем пользователей
    User u1("U1001", "Alice", "alice@example.com");
    User u2("U1002", "Bob", "bob@example.com");
    
    // Создаем проект
    Project p("P1001", "Website Redesign");
    p.addMember(u1);
    p.addMember(u2);
    
    // Добавляем задачи
    Task t1("T1001", "Design homepage", "Create new design");
    t1.assignTo(&u1);
    t1.logHours(5);
    p.addTask(t1);
    
    // Настраиваем интеграцию с API
    ExternalAPI jira("Jira", "https://api.jira.com");
    jira.setConfig("api_key", "12345");
    p.addIntegration(jira);
    
    // Синхронизируем данные
    p.syncWithAPI("Jira");
    
    // Генерируем отчет
    p.generateTimeReport();
    
    return 0;
}