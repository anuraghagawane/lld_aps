#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Menu {
public:
  virtual void display(string indent) = 0;
  virtual int getItemCount() = 0;
  virtual ~Menu() {}
};

class MenuItem : public Menu {
private:
  string name;
  double price;

public:
  MenuItem(string name, double price) : name(name), price(price) {}

  void display(string indent) override {
    printf("%s%s - $%.2f\n", indent.c_str(), name.c_str(), price);
  }

  int getItemCount() override { return 1; }
};

class SubMenu : public Menu {
  // TODO: Add a field to store the submenu name (string)
  // TODO: Add a field to store the list of Menu* children (vector<Menu*>)
  string name;
  vector<Menu *> items;

public:
  SubMenu(string name) {
    // TODO: Store the name
    this->name = name;
  }

  void addItem(Menu *item) {
    // TODO: Add the given item to the children list
    items.push_back(item);
  }

  void display(string indent) override {
    // TODO: Print the submenu name with indent + colon
    // TODO: Loop through children, calling child->display() with indent + "  "
    printf("%s%s:\n", indent.c_str(), name.c_str());
    for (auto item : items)
      item->display(indent + "  ");
  }

  int getItemCount() override {
    // TODO: Sum all children's getItemCount()
    int total = 0;
    for (auto item : items)
      total += item->getItemCount();
    return total;
  }
};

int main() {
  MenuItem burger("Burger", 8.99);
  MenuItem fries("Fries", 3.99);
  MenuItem cola("Cola", 1.99);
  MenuItem water("Water", 0.99);

  SubMenu drinks("Drinks");
  drinks.addItem(&cola);
  drinks.addItem(&water);

  SubMenu mainMenu("Main Menu");
  mainMenu.addItem(&burger);
  mainMenu.addItem(&fries);
  mainMenu.addItem(&drinks);

  mainMenu.display("");
  cout << "\nTotal items: " << mainMenu.getItemCount() << endl;

  return 0;
}
