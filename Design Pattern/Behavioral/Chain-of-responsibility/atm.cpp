#include <iostream>

using namespace std;

class CashRequest {
public:
  int amount;

  CashRequest(int amount) : amount(amount) {}
};

class CashHandler {
public:
  virtual void setNext(CashHandler *next) = 0;
  virtual void dispense(CashRequest &request) = 0;
  virtual ~CashHandler() {}
};

class BaseCashHandler : public CashHandler {
protected:
  CashHandler *next;
  int denomination;

public:
  BaseCashHandler(int denomination)
      : next(nullptr), denomination(denomination) {}

  void setNext(CashHandler *next) override { this->next = next; }

  void dispense(CashRequest &request) override {
    if (request.amount >= denomination) {
      int noteCount = request.amount / denomination;
      request.amount = request.amount % denomination;
      cout << "Dispensing " << noteCount << " x $" << denomination << endl;
    }

    forward(request);
  }

protected:
  void forward(CashRequest &request) {
    if (next != nullptr) {
      next->dispense(request);
    }
  }
};

class HundredDollarHandler : public BaseCashHandler {
public:
  HundredDollarHandler() : BaseCashHandler(100) {}
};

class FiftyDollarHandler : public BaseCashHandler {
public:
  FiftyDollarHandler() : BaseCashHandler(50) {}
};

class TwentyDollarHandler : public BaseCashHandler {
public:
  TwentyDollarHandler() : BaseCashHandler(20) {}
};

class TenDollarHandler : public BaseCashHandler {
public:
  TenDollarHandler() : BaseCashHandler(10) {}
};

int main() {
  HundredDollarHandler hundreds;
  FiftyDollarHandler fifties;
  TwentyDollarHandler twenties;
  TenDollarHandler tens;

  hundreds.setNext(&fifties);
  fifties.setNext(&twenties);
  twenties.setNext(&tens);

  cout << "--- Withdrawing $380 ---" << endl;
  CashRequest request1(380);
  hundreds.dispense(request1);
  cout << "Remaining: $" << request1.amount << endl;

  cout << "\n--- Withdrawing $275 ---" << endl;
  CashRequest request2(275);
  hundreds.dispense(request2);
  cout << "Remaining: $" << request2.amount << endl;

  return 0;
}
