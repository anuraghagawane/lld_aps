#include <iostream>
#include <queue>
#include <stack>
#include <string>
using namespace std;

class OrderCommand {
public:
  virtual ~OrderCommand() = default;
  virtual void execute() = 0;
  virtual void undo() = 0;
};

class Kitchen {
public:
  void prepareDish(const string &dish) {
    // TODO: Print "Preparing: [dish]"
    cout << "Preparing: " << dish << endl;
  }

  void cancelDish(const string &dish) {
    // TODO: Print "Cancelling: [dish]"
    cout << "Cancelling: " << dish << endl;
  }
};

class PlaceOrderCommand : public OrderCommand {
  // TODO: Add fields (kitchen, dish)
  Kitchen *kitchen;
  string dish;

public:
  PlaceOrderCommand(Kitchen *kitchen, const string &dish) {
    // TODO: Store receiver and dish
    this->kitchen = kitchen;
    this->dish = dish;
  }

  void execute() override {
    // TODO: Call kitchen->prepareDish(dish)
    kitchen->prepareDish(dish);
  }

  void undo() override {
    // TODO: Call kitchen->cancelDish(dish)
    kitchen->cancelDish(dish);
  }
};

class CancelOrderCommand : public OrderCommand {
  // TODO: Add fields (kitchen, dish)
  Kitchen *kitchen;
  string dish;

public:
  CancelOrderCommand(Kitchen *kitchen, const string &dish) {
    // TODO: Store receiver and dish
    this->kitchen = kitchen;
    this->dish = dish;
  }

  void execute() override {
    // TODO: Call kitchen->cancelDish(dish)
    kitchen->cancelDish(dish);
  }

  void undo() override {
    // TODO: Call kitchen->prepareDish(dish)
    kitchen->prepareDish(dish);
  }
};

class Waiter {
  // TODO: Add a pending queue and a history stack
  queue<OrderCommand *> pending;
  stack<OrderCommand *> history;

public:
  void takeOrder(OrderCommand *command) {
    // TODO: Add command to pending queue
    pending.push(command);
  }

  void submitOrders() {
    // TODO: Execute all pending commands, move them to history
    while (!pending.empty()) {
      auto front = pending.front();
      pending.pop();
      front->execute();
      history.push(front);
    }
  }

  void undoLast() {
    // TODO: Pop the most recent command from history and call undo()
    auto top = history.top();
    history.pop();
    top->undo();
  }
};

int main() {
  Kitchen kitchen;
  Waiter waiter;
  waiter.takeOrder(new PlaceOrderCommand(&kitchen, "Pasta"));
  waiter.takeOrder(new PlaceOrderCommand(&kitchen, "Salad"));
  waiter.submitOrders();
  waiter.takeOrder(new CancelOrderCommand(&kitchen, "Salad"));
  waiter.submitOrders();
  waiter.undoLast();
  return 0;
}
