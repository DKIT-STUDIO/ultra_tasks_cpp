#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

// Класс Permission (Разрешение)
class Permission {
private:
    string name;
    string description;
public:
    Permission(const string& name, const string& description) 
        : name(name), description(description) {}
    
    string getName() const { return name; }
    string getDescription() const { return description; }
};

// Класс Role (Роль)
class Role {
private:
    string name;
    vector<Permission> permissions;
public:
    Role(const string& name) : name(name) {}
    
    void addPermission(const Permission& permission) {
        permissions.push_back(permission);
    }
    
    bool hasPermission(const string& permissionName) const {
        for (const auto& perm : permissions) {
            if (perm.getName() == permissionName) {
                return true;
            }
        }
        return false;
    }
    
    string getName() const { return name; }
};

// Класс User (Пользователь)
class User {
private:
    string username;
    string email;
    vector<Role> roles;
public:
    User(const string& username, const string& email) 
        : username(username), email(email) {}
    
    void addRole(const Role& role) {
        roles.push_back(role);
    }
    
    bool hasPermission(const string& permissionName) const {
        for (const auto& role : roles) {
            if (role.hasPermission(permissionName)) {
                return true;
            }
        }
        return false;
    }
    
    string getUsername() const { return username; }
    string getEmail() const { return email; }
};

// Класс Comment (Комментарий)
class Comment {
private:
    User* author;
    string content;
    string timestamp;
public:
    Comment(User* author, const string& content, const string& timestamp)
        : author(author), content(content), timestamp(timestamp) {}
    
    User* getAuthor() const { return author; }
    string getContent() const { return content; }
    string getTimestamp() const { return timestamp; }
};

// Класс Task (Задача)
class Task {
private:
    string id;
    string title;
    string description;
    string status;
    User* assignee;
    vector<Comment> comments;
public:
    Task(const string& id, const string& title, const string& description)
        : id(id), title(title), description(description), status("Open"), assignee(nullptr) {}
    
    void assignTo(User* user) {
        assignee = user;
    }
    
    void addComment(const Comment& comment) {
        comments.push_back(comment);
    }
    
    void updateStatus(const string& newStatus) {
        status = newStatus;
    }
    
    string getId() const { return id; }
    string getTitle() const { return title; }
    User* getAssignee() const { return assignee; }
    vector<Comment> getComments() const { return comments; }
};

// Класс Project (Проект)
class Project {
private:
    string id;
    string name;
    string description;
    vector<Task> tasks;
    vector<User*> members;
    vector<Comment> comments;
public:
    Project(const string& id, const string& name, const string& description)
        : id(id), name(name), description(description) {}
    
    void addMember(User* user) {
        members.push_back(user);
    }
    
    void addTask(const Task& task) {
        tasks.push_back(task);
    }
    
    void addComment(const Comment& comment) {
        comments.push_back(comment);
    }
    
    vector<Task> getTasks() const { return tasks; }
    vector<User*> getMembers() const { return members; }
    string getName() const { return name; }
};

// Система уведомлений
class NotificationSystem {
private:
    map<User*, vector<string>> notifications;
public:
    void notify(User* user, const string& message) {
        notifications[user].push_back(message);
    }
    
    vector<string> getNotifications(User* user) {
        return notifications[user];
    }
    
    void clearNotifications(User* user) {
        notifications[user].clear();
    }
};

int main() {
    // Создаем разрешения
    Permission createTask("create_task", "Create new tasks");
    Permission assignTask("assign_task", "Assign tasks to users");
    Permission commentTask("comment_task", "Add comments to tasks");
    
    // Создаем роли
    Role admin("Admin");
    admin.addPermission(createTask);
    admin.addPermission(assignTask);
    admin.addPermission(commentTask);
    
    Role developer("Developer");
    developer.addPermission(commentTask);
    
    // Создаем пользователей
    User user1("john_doe", "john@example.com");
    user1.addRole(admin);
    
    User user2("jane_smith", "jane@example.com");
    user2.addRole(developer);
    
    // Создаем проект
    Project project("proj1", "Website Redesign", "Redesign company website");
    project.addMember(&user1);
    project.addMember(&user2);
    
    // Создаем задачу
    Task task("task1", "Design homepage", "Create new design for homepage");
    project.addTask(task);
    
    // Добавляем комментарий
    Comment comment(&user2, "I have some questions about this task", "2023-05-01 10:00");
    project.getTasks()[0].addComment(comment);
    
    // Проверяем разрешения
    cout << user1.getUsername() << " can create tasks: " 
         << (user1.hasPermission("create_task") ? "Yes" : "No") << endl;
    cout << user2.getUsername() << " can create tasks: " 
         << (user2.hasPermission("create_task") ? "Yes" : "No") << endl;
    
    return 0;
}