#include <iostream>
#include <queue>
#include <thread>
#include <mutex>

using namespace std;

class Service {
protected:
    string serviceID;
public:
    Service(const string &sid)
        : serviceID(sid) {}
    virtual ~Service() {};
    string getServiceID() const { return serviceID; };
    virtual void processRequest(const string &request) = 0;
};

// Производный класс сервиса
class ConcreteService : public Service {
public:
    ConcreteService(const string &sid) : Service(sid) {}
    void processRequest(const string &request) override {
        cout << "Обрабатываю запрос: " << request << endl;
    }
};

class API {
protected:
    queue<string> requests;
    mutex mtx;
public:
    API() {}
    virtual ~API() {};
    void enqueueRequest(const string &req) {
        lock_guard<mutex> lck(mtx);
        requests.push(req);
    }
    string dequeueRequest() {
        lock_guard<mutex> lck(mtx);
        if (!requests.empty()) {
            string req = requests.front();
            requests.pop();
            return req;
        }
        return "";
    }
};

class Database {
protected:
    string dbName;
public:
    Database(const string &dbnm)
        : dbName(dbnm) {}
    virtual ~Database() {};
    void storeData(const string &data) {
        cout << "Хранение данных '" << data << "' в базе данных '" << dbName << "'\n";
    }
};

class MessageQueue {
protected:
    queue<string> messages;
    mutex mtx;
public:
    MessageQueue() {}
    virtual ~MessageQueue() {};
    void pushMessage(const string &msg) {
        lock_guard<mutex> lck(mtx);
        messages.push(msg);
    }
    string popMessage() {
        lock_guard<mutex> lck(mtx);
        if (!messages.empty()) {
            string msg = messages.front();
            messages.pop();
            return msg;
        }
        return "";
    }
};

class LoadBalancer {
protected:
    vector<Service*> services;
public:
    LoadBalancer() {}
    virtual ~LoadBalancer() {};
    void registerService(Service *svc) {
        services.push_back(svc);
    }
    Service* selectService() {
        if (!services.empty()) {
            return services[rand() % services.size()];
        }
        return nullptr;
    }
};

class Client {
protected:
    API *api;
public:
    Client(API *a)
        : api(a) {}
    virtual ~Client() {};
    void makeRequest(const string &req) {
        api->enqueueRequest(req);
    }
};

int main() {
    // Конкретные сервисы
    ConcreteService svc1("SVC1");
    ConcreteService svc2("SVC2");

    // База данных
    Database db("MySQL");

    // Очередь сообщений
    MessageQueue mq;

    // Балансировщик нагрузки
    LoadBalancer lb;
    lb.registerService(&svc1);
    lb.registerService(&svc2);

    // Интерфейс API
    API api;

    // Клиент
    Client client(&api);

    // Отправка запросов клиентом
    client.makeRequest("Запрос #1");
    client.makeRequest("Запрос #2");

    // Обработка запросов сервисом
    while (true) {
        string request = api.dequeueRequest();
        if (request.empty()) break;
        Service *selectedSvc = lb.selectService();
        selectedSvc->processRequest(request);
        mq.pushMessage(selectedSvc->getServiceID() + " обработал запрос: " + request);
    }

    // Извлечение сообщений из очереди
    while (true) {
        string msg = mq.popMessage();
        if (msg.empty()) break;
        db.storeData(msg);
    }

    return 0;
}