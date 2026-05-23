#include <atomic>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <unordered_map>

using namespace std;

enum class Denomination {
  ONE = 1,
  TWO = 2,
  FIVE = 5,
  TEN = 10,
  TWENTY = 20,
  FIFTY = 50,
  HUNDRED = 100,
  FIVE_HUNDRED = 500
};

int denominationValue(Denomination d) { return static_cast<int>(d); }

enum class TransactionStatus {
  CREATED,
  PAYMENT_PENDING,
  PAYMENT_COMPLETED,
  DISPENSED,
  CANCELLED,
  FAILED
};

class Product {
private:
  string id;
  string name;
  int price;
  atomic<int> quantity;

public:
  Product(string id, string name, int price, int quantity)
      : id(id), name(name), price(price), quantity(quantity) {}

  const string getId() const { return id; }
  const string getName() const { return name; }
  int getPrice() const { return price; }
  int getQuantity() const { return quantity; }

  void reduceQuantity() {
    if (quantity <= 0) {
      throw runtime_error("Out of stock");
    }

    quantity.fetch_sub(1);
  }

  void restock(int qty) { quantity.fetch_add(qty); }
};

class ProductInventory {
private:
  unordered_map<string, shared_ptr<Product>> products;
  mutable mutex mtx;

public:
  void addProduct(shared_ptr<Product> product) {
    lock_guard<mutex> lock(mtx);
    products[product->getId()] = product;
  }

  shared_ptr<Product> getProduct(const string &productId) {
    lock_guard<mutex> lock(mtx);

    if (!products.count(productId)) {
      return nullptr;
    }

    return products[productId];
  }

  void reduceStock(const string &productId) {
    lock_guard<mutex> lock(mtx);
    if (!products.count(productId)) {
      throw runtime_error("Invalid product");
    }

    products[productId]->reduceQuantity();
  }

  void restock(const string &productId, int qty) {
    lock_guard<mutex> lock(mtx);

    if (!products.count(productId)) {
      throw runtime_error("Invalid product");
    }

    products[productId]->restock(qty);
  }
};

class CashInventory {
private:
  map<int, int, greater<int>> cash;
  mutable mutex mtx;

public:
  void addCash(Denomination denomination, int count = 1) {
    lock_guard<mutex> lock(mtx);

    cash[denominationValue(denomination)] += count;
  }

  bool canReturnChange(int amount) {
    lock_guard<mutex> lock(mtx);

    int remaining = amount;

    for (auto &[denom, count] : cash) {
      int needed = remaining / denom;
      int used = min(needed, count);
      remaining -= used * denom;
    }

    return remaining == 0;
  }

  map<int, int> calculateChange(int amount) {
    lock_guard<mutex> lock(mtx);

    int remaining = amount;

    map<int, int> result;

    for (auto &[denom, count] : cash) {
      int needed = remaining / denom;
      int used = min(needed, count);

      if (used > 0) {
        result[denom] = used;
        remaining -= used * denom;
      }
    }
    if (remaining != 0) {
      throw runtime_error("Cannot return exact change");
    }
    return result;
  }

  void deductCash(const map<int, int> &change) {
    lock_guard<mutex> lock(mtx);

    for (auto &[denom, count] : change) {
      cash[denom] -= count;
    }
  }

  void showCashInventory() {
    lock_guard<mutex> lock(mtx);

    cout << "\n===== CASH INVENTORY =====\n";

    for (auto &[denom, count] : cash) {
      cout << denom << " => " << count << endl;
    }
  }
};

class Transaction {
private:
  shared_ptr<Product> product;
  int insertedAmount;
  int changedAmount;

  TransactionStatus status;

public:
  Transaction(shared_ptr<Product> product)
      : product(product), insertedAmount(0), changedAmount(0),
        status(TransactionStatus::CREATED) {}

  void addMoney(int amount) { insertedAmount += amount; }

  int getInsertedAmount() const { return insertedAmount; }

  shared_ptr<Product> getProduct() const { return product; }

  int getRemainingAmount() const {
    return product->getPrice() - insertedAmount;
  }

  bool isPaymentCompleted() const {
    return insertedAmount >= product->getPrice();
  }

  int getChangeAmount() const { return insertedAmount - product->getPrice(); }

  void setStatus(TransactionStatus s) { status = s; }
};

class PaymentMethod {
public:
  virtual void pay(Transaction &transaction, Denomination denomination) = 0;
  virtual ~PaymentMethod() = default;
};

class CashPayment : public PaymentMethod {
public:
  void pay(Transaction &transaction, Denomination denomination) override {
    transaction.addMoney(denominationValue(denomination));
  }
};

class VendingMachine;

class VendingState {
public:
  virtual void selectProduct(VendingMachine &machine, const string &productId) {
    throw runtime_error("Invalid operation");
  }

  virtual void insertMoney(VendingMachine &machine, Denomination denomination) {
    throw runtime_error("Invalid operation");
  }

  virtual void dispense(VendingMachine &machine) {
    throw runtime_error("Invalid operation");
  }

  virtual void cancel(VendingMachine &machine) {
    throw runtime_error("Invalid operation");
  }

  virtual ~VendingState() = default;
};

class IdleState : public VendingState {
public:
  void selectProduct(VendingMachine &machine, const string &productId) override;
};

class ProductSelectedState : public VendingState {
public:
  void insertMoney(VendingMachine &machine, Denomination denomination) override;

  void cancel(VendingMachine &machine) override;
};

class PaymentCompletedState : public VendingState {
public:
  void dispense(VendingMachine &machine) override;
};

class VendingMachine {
private:
  friend class IdleState;
  friend class ProductSelectedState;
  friend class PaymentCompletedState;

  unique_ptr<VendingState> state;

  ProductInventory inventory;
  CashInventory cashInventory;

  unique_ptr<Transaction> currentTransaction;

  unique_ptr<PaymentMethod> paymentMethod;

  mutable mutex mtx;

  void changeState(unique_ptr<VendingState> newState) {
    state = move(newState);
  }

public:
  VendingMachine()
      : state(make_unique<IdleState>()),
        paymentMethod(make_unique<CashPayment>()) {}

  void addProduct(shared_ptr<Product> product) {
    inventory.addProduct(product);
  }

  void addCash(Denomination denomination, int count = 1) {
    cashInventory.addCash(denomination, count);
  }

  void selectProduct(const string &productId) {
    lock_guard<mutex> lock(mtx);
    state->selectProduct(*this, productId);
  }

  void insertMoney(Denomination denomination) {
    lock_guard<mutex> lock(mtx);
    state->insertMoney(*this, denomination);
  }

  void dispense() {
    lock_guard<mutex> lock(mtx);
    state->dispense(*this);
  }

  void cancel() {
    lock_guard<mutex> lock(mtx);
    state->cancel(*this);
  }

  void showCashInventory() { cashInventory.showCashInventory(); }
};

void IdleState::selectProduct(VendingMachine &machine,
                              const string &productId) {
  auto product = machine.inventory.getProduct(productId);

  if (!product) {
    throw runtime_error("Invalid product");
  }

  if (product->getQuantity() <= 0) {
    throw runtime_error("Product out of stock");
  }

  machine.currentTransaction = make_unique<Transaction>(product);
  machine.changeState(make_unique<ProductSelectedState>());

  cout << "Selected Product: " << product->getName()
       << " Price: " << product->getPrice() << endl;
}

void ProductSelectedState::insertMoney(VendingMachine &machine,
                                       Denomination denomination) {
  machine.paymentMethod->pay(*machine.currentTransaction, denomination);

  machine.cashInventory.addCash(denomination);

  cout << "Inserted: " << denominationValue(denomination) << endl;

  int remaining = machine.currentTransaction->getRemainingAmount();

  if (remaining > 0) {
    cout << "Remaining Amount: " << remaining << endl;
  }

  if (machine.currentTransaction->isPaymentCompleted()) {
    int change = machine.currentTransaction->getChangeAmount();

    if (!machine.cashInventory.canReturnChange(change)) {
      throw runtime_error("Cannot return exact change");
    }

    machine.changeState(make_unique<PaymentCompletedState>());

    cout << "Payment completed\n";
  }
}

void ProductSelectedState::cancel(VendingMachine &machine) {

  int refund = machine.currentTransaction->getInsertedAmount();

  cout << "Transaction Cancelled. Refund: " << refund << endl;

  machine.currentTransaction.reset();

  machine.changeState(make_unique<IdleState>());
}

void PaymentCompletedState::dispense(VendingMachine &machine) {
  auto product = machine.currentTransaction->getProduct();

  int changeAmount = machine.currentTransaction->getChangeAmount();

  auto change = machine.cashInventory.calculateChange(changeAmount);

  machine.inventory.reduceStock(product->getId());

  machine.cashInventory.deductCash(change);

  cout << "\nDispensing Product: " << product->getName() << endl;

  if (changeAmount > 0) {
    cout << "Returning Change: \n";

    for (auto &[denom, count] : change) {
      cout << denom << " x " << count << endl;
    }
  }

  machine.currentTransaction.reset();
  machine.changeState(make_unique<IdleState>());
}

int main() {
  VendingMachine vm;

  vm.addProduct(make_shared<Product>("P1", "Coke", 70, 5));
  vm.addProduct(make_shared<Product>("P2", "Pepsi", 50, 5));

  vm.addCash(Denomination::TEN, 10);
  vm.addCash(Denomination::TWENTY, 10);
  vm.addCash(Denomination::FIFTY, 10);

  try {
    vm.selectProduct("P1");
    vm.insertMoney(Denomination::FIFTY);
    vm.insertMoney(Denomination::FIFTY);
    // vm.insertMoney(Denomination::TWENTY);

    vm.dispense();
  } catch (const exception &e) {
    cout << "Error: " << e.what() << endl;
  }

  vm.showCashInventory();
  return 0;
}
