#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <unordered_map>
#include <vector>

using namespace std;

class User {
private:
  string id;
  string name;

public:
  User(const string &id, const string &name) : id(id), name(name) {}

  string getId() const { return id; }
  string getName() const { return name; }
};

class Split {
private:
  shared_ptr<User> user;
  double amount;

public:
  Split(shared_ptr<User> user, double amount) : user(user), amount(amount) {}

  shared_ptr<User> getUser() const { return user; }
  double getAmount() const { return amount; }
};

class SplitStrategy {
public:
  virtual vector<Split>
  calculateSplits(double amount, const vector<shared_ptr<User>> &participants,
                  const vector<double> &values = {}) = 0;

  virtual ~SplitStrategy() = default;
};

class EqualSplitStrategy : public SplitStrategy {
public:
  virtual vector<Split>
  calculateSplits(double amount, const vector<shared_ptr<User>> &participants,
                  const vector<double> &values = {}) override {
    vector<Split> splits;

    double splitAmount = amount / participants.size();

    for (auto &user : participants) {
      splits.emplace_back(user, splitAmount);
    }

    return splits;
  }
};

class ExactSplitStrategy : public SplitStrategy {
public:
  vector<Split> calculateSplits(double amount,
                                const vector<shared_ptr<User>> &participants,
                                const vector<double> &values = {}) override {
    vector<Split> splits;

    double total = 0;
    for (double val : values) {
      total += val;
    }

    if (total != amount) {
      throw runtime_error("Exact split total mismatch");
    }

    for (int i = 0; i < participants.size(); i++) {
      splits.emplace_back(participants[i], values[i]);
    }

    return splits;
  }
};

class PercentageSplitStrategy : public SplitStrategy {
public:
  vector<Split> calculateSplits(double amount,
                                const vector<shared_ptr<User>> &participants,
                                const vector<double> &values = {}) override {
    vector<Split> splits;

    double totalPercentage = 0;

    for (double p : values) {
      totalPercentage += p;
    }

    if (totalPercentage != 100) {
      throw runtime_error("Percentage total must be 100");
    }

    for (int i = 0; i < participants.size(); i++) {
      double share = (amount * values[i]) / 100.0;
      splits.emplace_back(participants[i], share);
    }

    return splits;
  }
};

class Expense {
private:
  string id;
  string description;
  double amount;

  shared_ptr<User> paidBy;

  vector<shared_ptr<User>> participants;
  vector<Split> splits;

  chrono::system_clock::time_point timestamp;

public:
  Expense(const string &id, const string &description, double amount,
          shared_ptr<User> paidBy, const vector<shared_ptr<User>> &participants,
          const vector<Split> &splits)
      : id(id), description(description), amount(amount), paidBy(paidBy),
        participants(participants), splits(splits),
        timestamp(chrono::system_clock::now()) {}

  shared_ptr<User> getPaidBy() const { return paidBy; }

  vector<Split> getSplits() const { return splits; }

  string getDescription() const { return description; }

  double getAmount() const { return amount; }
};

class Settlement {
private:
  shared_ptr<User> from;
  shared_ptr<User> to;
  double amount;

public:
  Settlement(shared_ptr<User> from, shared_ptr<User> to, double amount)
      : from(from), to(to), amount(amount) {}

  void print() const {
    cout << from->getName() << " paid " << to->getName() << " Rs. " << amount
         << endl;
  }
};

class Group {
private:
  string id;
  string name;

  vector<shared_ptr<User>> members;

  vector<shared_ptr<Expense>> expenses;

  vector<Settlement> settlements;

  unordered_map<string, unordered_map<string, double>> balances;

  mutable mutex mtx;

public:
  Group(const string &id, const string &name) : id(id), name(name) {}

  void addMember(shared_ptr<User> user) {
    lock_guard<mutex> lock(mtx);
    members.push_back(user);
  }

  void addExpense(shared_ptr<Expense> expense) {
    lock_guard<mutex> lock(mtx);

    expenses.push_back(expense);

    auto payer = expense->getPaidBy();

    for (const auto &split : expense->getSplits()) {
      auto user = split.getUser();

      double share = split.getAmount();

      if (user->getId() == payer->getId())
        continue;

      balances[user->getId()][payer->getId()] += share;
      balances[payer->getId()][user->getId()] -= share;
    }
  }

  void showBalances() {
    lock_guard<mutex> lock(mtx);

    cout << "\n===== BALANCES =====\n";

    for (auto &[user1, innerMap] : balances) {
      for (auto &[user2, amount] : innerMap) {

        if (amount > 0) {
          cout << user1 << " owes " << user2 << " : Rs. " << amount << endl;
        }
      }
    }
  }

  void settleBalance(shared_ptr<User> from, shared_ptr<User> to,
                     double amount) {
    lock_guard<mutex> lock(mtx);

    balances[from->getId()][to->getId()] -= amount;
    balances[to->getId()][from->getId()] += amount;

    settlements.emplace_back(from, to, amount);
  }

  void showExpenses() {
    lock_guard<mutex> lock(mtx);

    cout << "\n===== EXPENSE HISTORY =====\n";

    for (auto &expense : expenses) {
      cout << expense->getDescription() << " | Amount: Rs. "
           << expense->getAmount()
           << " | Paid By: " << expense->getPaidBy()->getName() << endl;
    }
  }

  void showSettlements() {
    lock_guard<mutex> lock(mtx);

    cout << "\n===== SETTLEMENT HISTORY =====\n";

    for (auto &settlement : settlements) {
      settlement.print();
    }
  }
};

class SplitwiseService {
private:
  unordered_map<string, shared_ptr<User>> users;
  unordered_map<string, shared_ptr<Group>> groups;

public:
  shared_ptr<User> createUser(const string &id, const string &name) {
    auto user = make_shared<User>(id, name);

    users[id] = user;

    return user;
  }

  shared_ptr<Group> createGroup(const string &id, const string &name) {
    auto group = make_shared<Group>(id, name);

    groups[id] = group;

    return group;
  }
};

int main() {
  SplitwiseService service;

  auto alice = service.createUser("u1", "Alice");
  auto bob = service.createUser("u2", "Bob");
  auto charlie = service.createUser("u3", "Charlie");

  auto tripGroup = service.createGroup("g1", "Goa Trip");

  tripGroup->addMember(alice);
  tripGroup->addMember(bob);
  tripGroup->addMember(charlie);

  EqualSplitStrategy equalStrategy;

  vector<shared_ptr<User>> participants = {alice, bob, charlie};

  auto splits = equalStrategy.calculateSplits(900, participants);

  auto expense =
      make_shared<Expense>("e1", "Dinner", 900, alice, participants, splits);

  tripGroup->addExpense(expense);
  tripGroup->showExpenses();
  tripGroup->showBalances();

  tripGroup->settleBalance(bob, alice, 300);

  tripGroup->showSettlements();

  tripGroup->showBalances();
  return 0;
}
