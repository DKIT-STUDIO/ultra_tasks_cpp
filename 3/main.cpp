#include <iostream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

class Product {
private:
    string id;
    string name;
    string description;
    double price;
    int stock;
public:
    Product(const string& id, const string& name, const string& description, double price, int stock)
        : id(id), name(name), description(description), price(price), stock(stock) {}
    
    void reduceStock(int quantity) {
        stock -= quantity;
    }
    
    void increaseStock(int quantity) {
        stock += quantity;
    }
    
    string getId() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getStock() const { return stock; }
};

class Customer {
private:
    string id;
    string name;
    string email;
    string address;
public:
    Customer(const string& id, const string& name, const string& email, const string& address)
        : id(id), name(name), email(email), address(address) {}
    
    string getId() const { return id; }
    string getName() const { return name; }
    string getAddress() const { return address; }
};

class OrderItem {
private:
    Product* product;
    int quantity;
public:
    OrderItem(Product* product, int quantity) : product(product), quantity(quantity) {}
    
    double getTotalPrice() const {
        return product->getPrice() * quantity;
    }
    
    Product* getProduct() const { return product; }
    int getQuantity() const { return quantity; }
};

class Payment {
private:
    string id;
    string method;
    double amount;
    string status;
    time_t paymentDate;
public:
    Payment(const string& id, const string& method, double amount)
        : id(id), method(method), amount(amount), status("Pending"), paymentDate(0) {}
    
    void processPayment() {
        status = "Completed";
        paymentDate = time(nullptr);
    }
    
    string getStatus() const { return status; }
    double getAmount() const { return amount; }
};

class Shipping {
private:
    string id;
    string method;
    string address;
    string status;
    time_t shippingDate;
    time_t deliveryDate;
public:
    Shipping(const string& id, const string& method, const string& address)
        : id(id), method(method), address(address), status("Preparing"), shippingDate(0), deliveryDate(0) {}
    
    void ship() {
        status = "Shipped";
        shippingDate = time(nullptr);
        // Предполагаемая дата доставки - через 3 дня
        deliveryDate = shippingDate + (3 * 24 * 60 * 60);
    }
    
    void deliver() {
        status = "Delivered";
        deliveryDate = time(nullptr);
    }
    
    string getStatus() const { return status; }
};

class Order {
private:
    string id;
    Customer* customer;
    vector<OrderItem> items;
    Payment* payment;
    Shipping* shipping;
    string status;
    time_t orderDate;
public:
    Order(const string& id, Customer* customer)
        : id(id), customer(customer), status("Created"), payment(nullptr), shipping(nullptr) {
        orderDate = time(nullptr);
    }
    
    void addItem(Product* product, int quantity) {
        items.emplace_back(product, quantity);
        product->reduceStock(quantity);
    }
    
    void setPayment(Payment* payment) {
        this->payment = payment;
    }
    
    void setShipping(Shipping* shipping) {
        this->shipping = shipping;
    }
    
    void processOrder() {
        if (payment && payment->getStatus() == "Completed") {
            status = "Processing";
            if (shipping) {
                shipping->ship();
            }
        }
    }
    
    double getTotalPrice() const {
        double total = 0.0;
        for (const auto& item : items) {
            total += item.getTotalPrice();
        }
        return total;
    }
    
    string getStatus() const { return status; }
    vector<OrderItem> getItems() const { return items; }
};

class ShoppingCart {
private:
    Customer* customer;
    vector<pair<Product*, int>> items;
public:
    ShoppingCart(Customer* customer) : customer(customer) {}
    
    void addItem(Product* product, int quantity) {
        items.emplace_back(product, quantity);
    }
    
    void removeItem(Product* product) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (it->first == product) {
                items.erase(it);
                break;
            }
        }
    }
    
    Order* checkout(const string& orderId) {
        Order* order = new Order(orderId, customer);
        for (const auto& item : items) {
            order->addItem(item.first, item.second);
        }
        items.clear();
        return order;
    }
};

int main() {
    // Создаем продукты
    Product product1("P1001", "Laptop", "High-performance laptop", 999.99, 10);
    Product product2("P1002", "Smartphone", "Latest smartphone model", 699.99, 15);
    
    // Создаем клиента
    Customer customer("C2001", "John Doe", "john@example.com", "123 Main St, Anytown");
    
    // Создаем корзину и добавляем товары
    ShoppingCart cart(&customer);
    cart.addItem(&product1, 1);
    cart.addItem(&product2, 2);
    
    // Оформляем заказ
    Order* order = cart.checkout("ORD3001");
    
    // Создаем и обрабатываем платеж
    Payment payment("PAY4001", "Credit Card", order->getTotalPrice());
    payment.processPayment();
    order->setPayment(&payment);
    
    // Создаем доставку
    Shipping shipping("SH5001", "Express", customer.getAddress());
    order->setShipping(&shipping);
    
    // Обрабатываем заказ
    order->processOrder();
    
    // Выводим информацию о заказе
    cout << "Order ID: " << order->getStatus() << endl;
    cout << "Status: " << order->getStatus() << endl;
    cout << "Total: $" << order->getTotalPrice() << endl;
    
    return 0;
}