#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

enum class TransactionStatus { PENDING, SUCCESS, FAILED };

string generateTransactionId() {
  static int counter = 0;
  return "TXN_" + to_string(++counter);
}

class Account {
public:
  virtual string getId() const = 0;
  virtual string getCurrency() const = 0;
  virtual double getBalance() const = 0;
  virtual void debit(double amount) = 0;
  virtual void credit(double amount) = 0;
  virtual mutex &getMutex() = 0;
  virtual ~Account() = default;
};

class Wallet : public Account {
private:
  string id;
  string currency;
  double balance;
  mutable mutex mtx;

public:
  Wallet(const string &id, const string &currency, double balance = 0)
      : id(id), currency(currency), balance(balance) {}

  string getId() const override { return id; }

  string getCurrency() const override { return currency; }

  double getBalance() const override { return balance; }

  void debit(double amount) override {
    if (balance < amount) {
      throw runtime_error("Insufficient balance");
    }

    balance -= amount;
  }

  void credit(double amount) override { balance += amount; }

  mutex &getMutex() override { return mtx; }
};

class BankAccount : public Account {
private:
  string id;
  string bankName;
  string currency;
  double balance;

  mutable mutex mtx;

public:
  BankAccount(const string &id, const string &bankName, const string &currency,
              double balance = 0)
      : id(id), bankName(bankName), currency(currency), balance(balance) {}

  string getId() const override { return id; }

  string getCurrency() const override { return currency; }

  double getBalance() const override { return balance; }

  void debit(double amount) override {
    if (balance < amount) {
      throw runtime_error("Insufficient bank balance");
    }

    balance -= amount;
  }

  void credit(double amount) override { balance += amount; }

  mutex &getMutex() override { return mtx; }
};

class User {
private:
  string id;
  string name;
  vector<shared_ptr<Wallet>> wallets;

public:
  User(const string &id, const string &name) : id(id), name(name) {}

  void addWallet(shared_ptr<Wallet> wallet) { wallets.push_back(wallet); }

  string getId() const { return id; }

  string getName() const { return name; }
};

class Transaction {
private:
  string id;
  string sourceAccountId;
  string destinationAccountId;

  double sourceAmount;
  double destinationAmount;

  string sourceCurrency;
  string destinationCurrency;

  TransactionStatus status;

  chrono::system_clock::time_point timestamp;

public:
  Transaction(const string &sourceAccountId, const string &destinationAccountId,
              double sourceAmount, double destinationAmount,
              const string &sourceCurrency, const string &destinationCurrency)
      : id(generateTransactionId()), sourceAccountId(sourceAccountId),
        destinationAccountId(destinationAccountId), sourceAmount(sourceAmount),
        destinationAmount(destinationAmount), sourceCurrency(sourceCurrency),
        destinationCurrency(destinationCurrency),
        status(TransactionStatus::PENDING),
        timestamp(chrono::system_clock::now()) {}

  void markSuccess() { status = TransactionStatus::SUCCESS; }

  void markFailed() { status = TransactionStatus::FAILED; }

  void print() const {
    auto t = chrono::system_clock::to_time_t(timestamp);
    cout << "\n========== TRANSACTION ==========\n";
    cout << "ID: " << id << endl;
    cout << "FROM: " << sourceAccountId << endl;
    cout << "TO: " << destinationAccountId << endl;
    cout << "AMOUNT: " << sourceAmount << " " << sourceCurrency << endl;
    cout << "CREDITED: " << destinationAmount << " " << destinationCurrency
         << endl;
    cout << "STATUS: "
         << (status == TransactionStatus::SUCCESS ? "SUCCESS" : "FAILED")
         << endl;
    cout << "TIME: " << ctime(&t);
  }
};

class CurrencyConversionService {
private:
  unordered_map<string, unordered_map<string, double>> rates;

public:
  CurrencyConversionService() {
    rates["INR"]["USD"] = 0.012;
    rates["USD"]["INR"] = 83.0;

    rates["INR"]["EUR"] = 0.011;
    rates["EUR"]["INR"] = 90.0;
  }

  double convert(double amount, const string &from, const string &to) {
    if (from == to) {
      return amount;
    }

    if (!rates.count(from) || !rates[from].count(to)) {
      throw runtime_error("currency conversion unavailable");
    }

    return amount * rates[from][to];
  }
};

class TransferService {
private:
  CurrencyConversionService conversionService;
  vector<Transaction> transactions;
  mutable mutex historyMutex;

public:
  void transfer(shared_ptr<Account> source, shared_ptr<Account> destination,
                double amount) {
    scoped_lock lock(source->getMutex(), destination->getMutex());

    double convertedAmount = conversionService.convert(
        amount, source->getCurrency(), destination->getCurrency());

    Transaction txn(source->getId(), destination->getId(), amount,
                    convertedAmount, source->getCurrency(),
                    destination->getCurrency());

    try {

      source->debit(amount);
      destination->credit(convertedAmount);
      txn.markSuccess();
      {
        lock_guard<mutex> historyLock(historyMutex);
        transactions.push_back(txn);
      }

      cout << "\nTransfer Successful\n" << endl;
    } catch (...) {
      txn.markFailed();
      {
        lock_guard<mutex> historyLock(historyMutex);
        transactions.push_back(txn);
      }

      throw;
    }
  }

  void showTransactions() {

    lock_guard<mutex> lock(historyMutex);

    for (const auto &txn : transactions) {
      txn.print();
    }
  }
};

class DigitalWalletService {
private:
  unordered_map<string, shared_ptr<User>> users;
  TransferService transferService;

public:
  void addUser(shared_ptr<User> user) { users[user->getId()] = user; }

  void transfer(shared_ptr<Account> source, shared_ptr<Account> destination,
                double amount) {
    transferService.transfer(source, destination, amount);
  }

  void showTransactions() { transferService.showTransactions(); }
};

int main() {
  DigitalWalletService walletSystem;

  auto user1 = make_shared<User>("U1", "Anurag");
  auto user2 = make_shared<User>("U2", "Rahul");

  auto wallet1 = make_shared<Wallet>("W1", "INR", 10000);
  auto wallet2 = make_shared<Wallet>("W2", "USD", 100);

  auto bankAccount = make_shared<BankAccount>("B1", "ICICI", "INR", 5000);

  user1->addWallet(wallet1);
  user2->addWallet(wallet2);

  walletSystem.addUser(user1);
  walletSystem.addUser(user2);

  try {

    cout << "\nWallet1 Balance: " << wallet1->getBalance() << " INR\n";

    cout << "Wallet2 Balance: " << wallet2->getBalance() << " USD\n";

    walletSystem.transfer(wallet1, wallet2, 1000);

    walletSystem.transfer(wallet1, bankAccount, 2000);

    cout << "\nAfter Transfers\n";

    cout << "Wallet1: " << wallet1->getBalance() << " INR\n";

    cout << "Wallet2: " << wallet2->getBalance() << " USD\n";

    cout << "Bank Account: " << bankAccount->getBalance() << " INR\n";

  } catch (const exception &e) {

    cout << "Error: " << e.what() << endl;
  }

  walletSystem.showTransactions();
  return 0;
}
