#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

class Account {
  public:
    string accountNumber;
    string accountHolder;
    float balance;

    Account(string number, string holder, float bal) {
      accountNumber = number;
      accountHolder = holder;
      balance = bal;
    }

    virtual void displayDetails() {
      cout << "Account Details for Account (ID: " << accountNumber << "):" << endl;
      cout << "\tHolder: " << accountHolder << endl;
      cout << "\tBalance: $" << fixed << setprecision(2) << balance << endl;
    };

    virtual void deposit(int amount) {
      balance += amount;
    };

    virtual void withdraw(int amount) {
      int newBalance = balance - amount;
      if (newBalance >= 0) {
        balance = newBalance;
      } else {
        throw std::domain_error("Withdrew too much!");
      }
    };

    Account operator+(Account &other)
    {
        Account res = *this;
        other.withdraw(100);
        res.deposit(100);
        return res;
    }

    friend ostream &operator << (ostream &os, Account &account) {
      account.displayDetails();
      return os;
    }
};

class SavingsAccount: public Account {
  public:
    float interestRate;

    SavingsAccount(string number, string holder, int balance, float rate): Account(number, holder, balance) {
      interestRate = rate;
    }

    void displayDetails() {
      cout << "Account Details for Savings Account (ID: " << accountNumber << "):" << endl;
      cout << "\tHolder: " << accountHolder << endl;
      cout << "\tBalance: $" << fixed << setprecision(2) << balance << endl;
      cout << "\tInterest Rate: " << fixed << setprecision(2) << interestRate * 100 << "%" << endl;
    };

    // Enforce a minimum balance of $100.00
    void withdraw(int amount) {
      int newBalance = balance - amount;
      if (newBalance >= 100.00) {
        balance = newBalance;
      } else {
        throw std::domain_error("Withdrew too much!");
      }
    };

    SavingsAccount operator+(Account&other)
    {
        SavingsAccount res = *this;
        other.withdraw(100);
        res.deposit(100);
        return res;
    }
};

class CurrentAccount: public Account {
  public:
    float overdraftLimit;

    CurrentAccount(string number, string holder, int balance, float limit): Account(number, holder, balance) {
      overdraftLimit = limit;
    }

    void displayDetails() {
      cout << "Account Details for Current Account (ID: " << accountNumber << "):" << endl;
      cout << "\tHolder: " << accountHolder << endl;
      cout << "\tBalance: $" << fixed << setprecision(2) << balance << endl;
      cout << "\tOverdraft Limit: " << fixed << setprecision(2) << overdraftLimit << endl;
    };
  
    void withdraw(int amount) {
      int newBalance =  balance - amount;
      if (newBalance >= -overdraftLimit) {
        balance = newBalance;
      } else {
        throw std::domain_error("Withdrew too much!");
      }
    };

    CurrentAccount operator+(Account&other)
    {
        CurrentAccount res = *this;
        other.withdraw(100);
        res.deposit(100);
        return res;
    }
};

int main() {
    SavingsAccount savings("S123", "John Doe", 1000, 0.02);
    CurrentAccount current("C456", "Jane Doe", 2000, 500);

    cout << "Initial details:" << endl;
    cout << savings;
    cout << current;

    cout << endl << "Post deposit/withdraw details:" << endl;
    savings.deposit(250);
    current.withdraw(250);
    
    cout << savings;
    cout << current;

    cout << endl << "Post second deposit/withdraw details:" << endl;
    savings.withdraw(250);
    current.deposit(250);
    
    cout << savings;
    cout << current;

    // Transfer 100 from savings to current
    cout << endl << "Post transfer details:" << endl;
    current = current + savings;
    cout << savings;
    cout << current;

    // Transfer 100 back from current to savings
    cout << endl << "Post second transfer details:" << endl;
    savings = savings + current;

    // Use overloaded <<
    cout << savings;
    cout << current;

    cout << endl << "Attempt invalid withdraw for SavingsAccount:" << endl;
    try {
      // 1000 - 901 = 99 (< 100)
      savings.withdraw(901);
    } catch (const std::exception&) {
      cout << "Caught invalid withdrawal!" << endl;
    }

    cout << endl << "Attempt invalid withdraw for CurrentAccount:" << endl;
    try {
      // 2000 - 2501 = -501 (< -500)
      current.withdraw(2501);
    } catch (const std::exception&) {
      cout << "Caught invalid withdrawal!" << endl;
    }

    return 0;
}
