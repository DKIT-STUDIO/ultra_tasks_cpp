#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

class Currency {
protected:
    string code;
    double exchangeRate;
public:
    Currency(const string &c, double exchR)
        : code(c), exchangeRate(exchR) {}
    virtual ~Currency() {};
    string getCode() const { return code; };
    double getExchangeRate() const { return exchangeRate; };
};

class Transaction {
protected:
    double amount;
    string description;
    Currency *currency;
public:
    Transaction(double amnt, const string &desc, Currency *cur)
        : amount(amnt), description(desc), currency(cur) {}
    virtual ~Transaction() {};
    double getAmount() const { return amount; };
    string getDescription() const { return description; };
    Currency* getCurrency() const { return currency; };
};

class Account {
protected:
    string accountNumber;
    double balance;
    vector<Transaction*> transactions;
public:
    Account(const string &acctNumb, double bal)
        : accountNumber(acctNumb), balance(bal) {}
    virtual ~Account() {};
    void deposit(Transaction *trx) {
        balance += trx->getAmount();
        transactions.push_back(trx);
    }
    void withdraw(Transaction *trx) {
        if (balance >= trx->getAmount())
            balance -= trx->getAmount();
        else
            cerr << "Недостаточно средств.\n";
        transactions.push_back(trx);
    }
    double getBalance() const { return balance; };
    string getAccountNumber() const { return accountNumber; }; // Новый метод
};

class Budget {
protected:
    double limit;
    double spent;
public:
    Budget(double lim)
        : limit(lim), spent(0) {}
    virtual ~Budget() {};
    bool spend(double amount) {
        if ((spent + amount) <= limit) {
            spent += amount;
            return true;
        }
        return false;
    }
    double remaining() const { return limit - spent; };
};

class Report {
protected:
    vector<Account*> accounts;
public:
    Report(vector<Account*> accts)
        : accounts(accts) {}
    virtual ~Report() {};
    void printSummary() const {
        for (auto acc : accounts) {
            cout << "Аккаунт: " << acc->getAccountNumber() << ", Баланс: $" << fixed << setprecision(2) << acc->getBalance() << "\n"; // Используем новый метод
        }
    }
};

int main() {
    // Валюта
    Currency usd("USD", 1.0);
    Currency rub("RUB", 80.0);

    // Транзакции
    Transaction purchase(100.0, "Покупка товаров", &usd);
    Transaction payment(500.0, "Оплата услуг", &rub);

    // Банковский счёт
    Account myAccount("ACC12345", 1000.0);
    myAccount.deposit(&purchase);
    myAccount.withdraw(&payment);

    // Бюджет
    Budget monthlyBudget(1500.0);
    monthlyBudget.spend(myAccount.getBalance());

    // Отчёт
    Report financialReport({&myAccount});
    financialReport.printSummary();

    return 0;
}