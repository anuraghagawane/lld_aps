#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>

using namespace std;

class Account {
private:
  string accountId;
  int balance;

public:
  Account(string accountId, int balance)
      : accountId(accountId), balance(balance) {}
  int getBalance() const { return balance; }

  bool debit(int amount) {
    if (balance < amount)
      return false;
    balance -= amount;
    return true;
  }

  void credit(int amount) {
    if (amount > 0)
      balance += amount;
  }
};

class Card {
private:
  string cardNumber;
  string pin;
  Account *account;

public:
  Card(string cardNumber, string pin, Account *account)
      : cardNumber(cardNumber), pin(pin), account(account) {}

  string getCardNumber() const { return cardNumber; }

  bool validatePin(string pin) const { return this->pin == pin; }
  Account *getAccount() { return account; }
};

class BankService {
private:
  unordered_map<string, Card *> cards;

public:
  void addCard(Card *card) { cards[card->getCardNumber()] = card; }

  Account *authenticate(string cardNumber, string pin) {
    if (cards.count(cardNumber) && cards[cardNumber]->validatePin(pin)) {
      return cards[cardNumber]->getAccount();
    }

    return nullptr;
  }
};

class CashDispenser {
private:
  map<int, int> notes;

public:
  CashDispenser() {
    notes[2000] = 10;
    notes[500] = 20;
    notes[100] = 50;
  }

  bool canDispense(int amount) const {
    int remaining = amount;
    for (auto it = notes.rbegin(); it != notes.rend(); ++it) {
      int denom = it->first;
      int count = it->second;

      int needed = remaining / denom;
      int used = min(needed, count);
      remaining -= used * denom;
    }
    return remaining == 0;
  }

  void dispense(int amount) {
    int remaining = amount;
    for (auto it = notes.rbegin(); it != notes.rend(); ++it) {
      int denom = it->first;
      int count = it->second;

      int needed = remaining / denom;
      int used = min(needed, count);
      it->second -= used;
      remaining -= used * denom;

      if (used > 0) {
        cout << "Dispensed " << used << " notes of " << denom << endl;
      }
    }
  }
};

class Transaction {
public:
  virtual void execute() = 0;
  virtual ~Transaction() {}
};

class WithdrawTransaction : public Transaction {
private:
  Account *account;
  int amount;
  CashDispenser *dispenser;

public:
  WithdrawTransaction(Account *account, int amount, CashDispenser *dispenser)
      : account(account), amount(amount), dispenser(dispenser) {}

  void execute() override {
    if (account->getBalance() < amount) {
      cout << "Insufficient balance\n";
      return;
    }

    if (!dispenser->canDispense(amount)) {
      cout << "ATM cannot dispense this amount\n";
      return;
    }

    account->debit(amount);
    dispenser->dispense(amount);
  }
};

class DepositTransaction : public Transaction {
private:
  Account *account;
  int amount;

public:
  DepositTransaction(Account *account, int amount)
      : account(account), amount(amount) {}

  void execute() override {
    if (amount <= 0) {
      cout << "Invalid Deposit Amount" << endl;
      return;
    }

    account->credit(amount);
    cout << "Deposit successfull" << endl;
  }
};
class ATM {
private:
  BankService *bankService;
  CashDispenser dispenser;
  Account *currentAccount;

public:
  ATM(BankService *bankService) : bankService(bankService) {}

  bool authenticate(string cardNumber, string pin) {
    currentAccount = bankService->authenticate(cardNumber, pin);
    if (currentAccount) {
      cout << "Authentication successfull\n";
      return true;
    }
    cout << "Authentication failed\n";
    return false;
  }

  void withdraw(int amount) {
    if (!currentAccount) {
      cout << "No active session" << endl;
      return;
    }
    unique_ptr<Transaction> txn =
        make_unique<WithdrawTransaction>(currentAccount, amount, &dispenser);
    txn->execute();
  }

  void deposit(int amount) {
    if (!currentAccount) {
      cout << "No active session" << endl;
      return;
    }

    unique_ptr<Transaction> txn =
        make_unique<DepositTransaction>(currentAccount, amount);
    txn->execute();
  }

  void endSession() { currentAccount = nullptr; }
};

int main() {
  cout << "ATM" << endl;
  Account acc1("A1", 10000);
  Card card1("1234", "1111", &acc1);

  BankService bank;
  bank.addCard(&card1);

  ATM atm(&bank);

  if (atm.authenticate("1234", "1111")) {
    atm.withdraw(3700);
    atm.deposit(2000);

    atm.endSession();
  }
  return 0;
}
