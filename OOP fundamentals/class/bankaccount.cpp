#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

class BankAccount {
private:
    // Add private fields: accountNumber (string), ownerName (string), balance (double)
    string accountNumber;
    string ownerName;
    double balance;

public:
    BankAccount(const string& accountNumber, const string& ownerName) :
        accountNumber(accountNumber), ownerName(ownerName), balance(0) {}

    void deposit(double amount) {
        // Add amount to balance (only if amount is positive)
        if(amount > 0) {
            balance += amount;
        }
    }

    bool withdraw(double amount) {
        // Remove amount from balance if sufficient funds exist
        // Return true if successful, false otherwise
        if(balance - amount >= 0) {
            balance -= amount;
            return true;
        }
        return false;
    }

    double getBalance() const {
        // Return the current balance
        return balance;
    }
};

int main() {
    BankAccount account("123456", "John Doe");
    account.deposit(1000);
    cout << fixed << setprecision(1) << account.getBalance() << endl;  // Should print 1000.0

    bool success = account.withdraw(500);
    cout << boolalpha << success << endl;  // Should print true
    cout << fixed << setprecision(1) << account.getBalance() << endl;  // Should print 500.0

    success = account.withdraw(1000);
    cout << boolalpha << success << endl;  // Should print false

    return 0;
}
