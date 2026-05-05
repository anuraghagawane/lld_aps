#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <unordered_map>

using namespace std;

class Account {
private:
  string accountId;
  int balance;
  mutable mutex mtx;

public:
  Account(string accountId, int balance)
      : accountId(accountId), balance(balance) {}
  int getBalance() const {
    lock_guard<mutex> lock(mtx);
    return balance;
  }

  bool debit(int amount) {
    lock_guard<mutex> lock(mtx);
    if (balance < amount)
      return false;
    balance -= amount;
    return true;
  }

  void credit(int amount) {
    lock_guard<mutex> lock(mtx);
    if (amount > 0)
      balance += amount;
  }
};

class Card {
private:
  string cardNumber;
  string pin;
  shared_ptr<Account> account;

public:
  Card(string cardNumber, string pin, shared_ptr<Account> account)
      : cardNumber(cardNumber), pin(pin), account(account) {}

  string getCardNumber() const { return cardNumber; }

  bool validatePin(string pin) const { return this->pin == pin; }
  shared_ptr<Account> getAccount() { return account; }
};

class BankService {
private:
  unordered_map<string, shared_ptr<Card>> cards;

public:
  void addCard(shared_ptr<Card> card) { cards[card->getCardNumber()] = card; }

  shared_ptr<Account> authenticate(const string cardNumber, const string pin) {
    if (cards.count(cardNumber) && cards[cardNumber]->validatePin(pin)) {
      return cards[cardNumber]->getAccount();
    }

    return nullptr;
  }
};

class CashDispenser {
private:
  map<int, int> notes;
  mutable mutex mtx;

public:
  CashDispenser() {
    notes[2000] = 10;
    notes[500] = 20;
    notes[100] = 50;
  }

  bool canDispense(int amount) const {
    lock_guard<mutex> lock(mtx);
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
    std::lock_guard<std::mutex> lock(mtx);
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
  shared_ptr<Account> account;
  int amount;
  shared_ptr<CashDispenser> dispenser;

public:
  WithdrawTransaction(shared_ptr<Account> account, int amount,
                      shared_ptr<CashDispenser> dispenser)
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
  shared_ptr<Account> account;
  int amount;

public:
  DepositTransaction(shared_ptr<Account> account, int amount)
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

class ATM;

class ATMState {
public:
  virtual void insertCard(ATM &atm, const string &cardNumber) {
    throw std::runtime_error("Card already inserted");
  }

  virtual void enterPin(ATM &atm, const string &pin) {
    throw runtime_error("PIN entry not allowed now");
  }

  virtual void withdraw(ATM &atm, int amount) {
    throw runtime_error("Not authenticated");
  }

  virtual void deposit(ATM &atm, int amount) {
    throw std::runtime_error("Not authenticated");
  }

  virtual void ejectCard(ATM &atm) {
    throw std::runtime_error("No card to eject");
  }

  virtual ~ATMState() = default;
};

class IdleState : public ATMState {
public:
  void insertCard(ATM &atm, const string &cardNumber) override;
};

class CardInsertedState : public ATMState {
public:
  void enterPin(ATM &atm, const string &amount) override;
  void ejectCard(ATM &atm) override;
};

class AuthenticatedState : public ATMState {
public:
  void withdraw(ATM &atm, int amount) override;
  void deposit(ATM &atm, int amount) override;
  void ejectCard(ATM &atm) override;
};

class ATM {
private:
  friend class IdleState;
  friend class CardInsertedState;
  friend class AuthenticatedState;
  unique_ptr<ATMState> state;
  shared_ptr<BankService> bankService;
  shared_ptr<CashDispenser> dispenser;
  shared_ptr<Account> currentAccount;
  string currentCardNumber;

  void changeState(unique_ptr<ATMState> newState) { state = move(newState); }

public:
  ATM(shared_ptr<BankService> bankService)
      : state(make_unique<IdleState>()), bankService(move(bankService)),
        dispenser(make_shared<CashDispenser>()) {}

  void insertCard(const string &cardNumber) {
    state->insertCard(*this, cardNumber);
  }

  void enterPin(const string &pin) { state->enterPin(*this, pin); }

  void withdraw(int amount) { state->withdraw(*this, amount); }

  void deposit(int amount) { state->deposit(*this, amount); }

  void ejectCard() { state->ejectCard(*this); }
};

void IdleState::insertCard(ATM &atm, const string &cardNumber) {
  atm.currentCardNumber = cardNumber;
  atm.changeState(make_unique<CardInsertedState>());
  cout << "Card inserted: " << cardNumber << "\n";
}

void CardInsertedState::enterPin(ATM &atm, const string &pin) {
  auto account = atm.bankService->authenticate(atm.currentCardNumber, pin);
  if (account) {
    atm.currentAccount = account;
    atm.changeState(make_unique<AuthenticatedState>());
    cout << "Authentication successfull" << endl;
  } else {
    cout << "Authentication failed" << endl;
    atm.currentCardNumber.clear();
    atm.changeState(make_unique<IdleState>());
  }
}

void CardInsertedState::ejectCard(ATM &atm) {
  std::cout << "Ejecting card: " << atm.currentCardNumber << "\n";
  atm.currentCardNumber.clear();
  atm.changeState(std::make_unique<IdleState>());
}

void AuthenticatedState::withdraw(ATM &atm, int amount) {
  if (!atm.currentAccount)
    return;
  auto txn = make_unique<WithdrawTransaction>(atm.currentAccount, amount,
                                              atm.dispenser);

  txn->execute();
}

void AuthenticatedState::deposit(ATM &atm, int amount) {
  if (!atm.currentAccount)
    return;
  auto txn = std::make_unique<DepositTransaction>(atm.currentAccount, amount);
  txn->execute();
}

void AuthenticatedState::ejectCard(ATM &atm) {
  std::cout << "Ejecting card: " << atm.currentCardNumber << "\n";
  atm.currentAccount.reset();
  atm.currentCardNumber.clear();
  atm.changeState(std::make_unique<IdleState>());
}

int main() {
  auto acc1 = make_shared<Account>("A1", 10000);
  auto card1 = make_shared<Card>("1234-5678", "1111", acc1);
  auto bank = make_shared<BankService>();

  bank->addCard(card1);

  ATM atm(bank);

  atm.insertCard("1234-5678");
  atm.enterPin("1111");
  atm.withdraw(2500);
  atm.deposit(800);
  atm.ejectCard();

  try {
    atm.withdraw(100);
  } catch (const std::runtime_error &e) {
    std::cout << "Error: " << e.what() << "\n";
  }

  return 0;
}
