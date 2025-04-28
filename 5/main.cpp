#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

class TeamMember {
private:
    string id;
    string name;
    string role;
public:
    TeamMember(const string& id, const string& name, const string& role)
        : id(id), name(name), role(role) {}
    
    string getId() const { return id; }
    string getName() const { return name; }
    string getRole() const { return role; }
};

class UserStory {
private:
    string id;
    string description;
    string status;
    int storyPoints;
public:
    UserStory(const string& id, const string& description, int storyPoints)
        : id(id), description(description), storyPoints(storyPoints), status("To Do") {}
    
    void updateStatus(const string& newStatus) {
        status = newStatus;
    }
    
    string getId() const { return id; }
    string getDescription() const { return description; }
    int getStoryPoints() const { return storyPoints; }
    string getStatus() const { return status; }
};

class Task {
private:
    string id;
    string description;
    UserStory* userStory;
    TeamMember* assignee;
    string status;
    int estimatedHours;
    int actualHours;
public:
    Task(const string& id, const string& description, UserStory* userStory, int estimatedHours)
        : id(id), description(description), userStory(userStory), estimatedHours(estimatedHours),
          actualHours(0), status("To Do"), assignee(nullptr) {}
    
    void assignTo(TeamMember* member) {
        assignee = member;
    }
    
    void updateStatus(const string& newStatus) {
        status = newStatus;
    }
    
    void logHours(int hours) {
        actualHours += hours;
    }
    
    string getId() const { return id; }
    TeamMember* getAssignee() const { return assignee; }
    int getEstimatedHours() const { return estimatedHours; }
    int getActualHours() const { return actualHours; }
};

class Sprint {
private:
    string id;
    string name;
    string startDate;
    string endDate;
    vector<UserStory*> userStories;
    vector<Task*> tasks;
public:
    Sprint(const string& id, const string& name, const string& start, const string& end)
        : id(id), name(name), startDate(start), endDate(end) {}
    
    void addUserStory(UserStory* userStory) {
        userStories.push_back(userStory);
    }
    
    void addTask(Task* task) {
        tasks.push_back(task);
    }
    
    int getTotalStoryPoints() const {
        int total = 0;
        for (const auto& story : userStories) {
            total += story->getStoryPoints();
        }
        return total;
    }
    
    int getCompletedStoryPoints() const {
        int total = 0;
        for (const auto& story : userStories) {
            if (story->getStatus() == "Done") {
                total += story->getStoryPoints();
            }
        }
        return total;
    }
    
    vector<UserStory*> getUserStories() const { return userStories; }
    vector<Task*> getTasks() const { return tasks; }
};

class Backlog {
private:
    vector<UserStory*> userStories;
public:
    void addUserStory(UserStory* userStory) {
        userStories.push_back(userStory);
    }
    
    void prioritizeUserStory(UserStory* userStory) {
        for (auto it = userStories.begin(); it != userStories.end(); ++it) {
            if (*it == userStory) {
                userStories.erase(it);
                userStories.insert(userStories.begin(), userStory);
                break;
            }
        }
    }
    
    vector<UserStory*> getUserStories() const { return userStories; }
};

class BurndownChart {
private:
    Sprint* sprint;
    map<string, pair<int, int>> dailyProgress; // date -> (remaining, completed)
public:
    BurndownChart(Sprint* sprint) : sprint(sprint) {}
    
    void updateProgress(const string& date, int remainingPoints, int completedPoints) {
        dailyProgress[date] = make_pair(remainingPoints, completedPoints);
    }
    
    void generateChart() const {
        cout << "Burndown Chart for Sprint: " << sprint->getTotalStoryPoints() << " points\n";
        cout << "Date\t\tRemaining\tCompleted\n";
        for (const auto& entry : dailyProgress) {
            cout << entry.first << "\t" << entry.second.first << "\t\t" << entry.second.second << endl;
        }
    }
};

int main() {
    // Создаем членов команды
    TeamMember dev1("TM001", "Alice", "Developer");
    TeamMember dev2("TM002", "Bob", "Developer");
    TeamMember po("TM003", "Charlie", "Product Owner");
    
    // Создаем бэклог
    Backlog backlog;
    
    // Добавляем пользовательские истории
    UserStory us1("US001", "As a user, I want to login", 5);
    UserStory us2("US002", "As a user, I want to view my profile", 3);
    backlog.addUserStory(&us1);
    backlog.addUserStory(&us2);
    
    // Приоритезируем истории
    backlog.prioritizeUserStory(&us2);
    
    // Создаем спринт
    Sprint sprint("SP001", "Sprint 1", "2023-05-01", "2023-05-14");
    sprint.addUserStory(&us2);
    
    // Создаем задачи
    Task task1("T001", "Implement login page", &us2, 8);
    Task task2("T002", "Create profile view", &us2, 6);
    task1.assignTo(&dev1);
    task2.assignTo(&dev2);
    
    sprint.addTask(&task1);
    sprint.addTask(&task2);
    
    // Обновляем статусы
    task1.updateStatus("In Progress");
    task1.logHours(4);
    
    // Создаем и обновляем Burndown Chart
    BurndownChart chart(&sprint);
    chart.updateProgress("2023-05-01", 3, 0);
    chart.updateProgress("2023-05-02", 3, 0);
    chart.updateProgress("2023-05-03", 2, 1);
    
    // Генерируем отчет
    chart.generateChart();
    
    return 0;
}