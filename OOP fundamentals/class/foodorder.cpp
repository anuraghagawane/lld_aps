#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

class FoodOrder {
private:
  std::string orderId;
  std::string customerName;
  std::vector<std::string> items;
  double totalAmount;
  bool isPlaced;

public:
  FoodOrder(const std::string& orderId, const std::string& customerName) 
    : orderId(orderId), customerName(customerName), totalAmount(0.0), isPlaced(false) {}

  void addItem(const std::string& name, double price) {
    if(isPlaced) {
      std::cout<<"Cannot modify a placed order"<<std::endl;
      return;
    }

    items.push_back(name);
    totalAmount += price;
  }

  bool placeOrder() {
    if(isPlaced || items.empty()) {
      return false;
    }
    isPlaced = true;
    return true;
  }

  int getItemCount() const {
    return items.size();
  }

  void displayOrder() const {
    std::string status = isPlaced ? "PLACED" : "PENDING";
    std::cout << "Order " << orderId << " (" << customerName<< ") - " << status << std::endl;
    for (const auto& item : items) {
        std::cout << "  - " << item << std::endl;
    }
    std::cout << "  Total: $" << std::fixed << std::setprecision(2)<< totalAmount << std::endl;
  }
};


int main() {
  FoodOrder order1("ORD-101", "Alice");
  order1.addItem("Pizza", 12.99);
  order1.addItem("Garlic Bread", 4.99);
  order1.addItem("Coke", 2.49);
  order1.placeOrder();

  FoodOrder order2("ORD-102", "Bob");
  order2.addItem("Burger", 9.99);
  order2.addItem("Fries", 3.99);

  order1.displayOrder();
  std::cout << std::endl;
  order2.displayOrder();
  return 0;
}
