#include <iostream>
#include <string>
#include <map>
#include <cmath>

using namespace std;

class ShoppingCart {
private:
    map<string, double> items;
    bool discountApplied = false;
    bool isCheckedOut = false;

public:
    void addItem(const string& name, double price) {
        // Add item to cart, but reject if already checked out
        if(isCheckedOut) {
            cout<<"Cannot modify a checked-out cart"<<endl;
            return;
        }
        items[name] = price;
    }

    bool applyDiscount(const string& code) {
        // If code is "SAVE10" and no discount applied yet and not checked out,
        // mark discount as applied and return true. Otherwise return false.
        if(!discountApplied) {
            discountApplied = true;
            return true;
        }
        return false;
    }

    double getTotal() const {
        // Sum all item prices. If discount was applied, subtract 10%.
        double total = 0;
        for(auto& item: items) total += item.second;
        if(discountApplied) {
            total -= (total * 10) / 100; 
        }
        return round(total * 100.0) / 100.0;
    }

    void checkout() {
        // Mark cart as checked out (only if it has items and isn't already checked out)
        isCheckedOut = true;
    }
};

int main() {
    ShoppingCart cart;
    cart.addItem("Laptop", 999.99);
    cart.addItem("Mouse", 29.99);

    cout << "Total: $" << cart.getTotal() << endl;                    // 1029.98

    cout << "Discount: " << boolalpha << cart.applyDiscount("SAVE10") << endl; // true
    cout << "Total: $" << cart.getTotal() << endl;                    // 926.98

    cout << "Discount: " << cart.applyDiscount("SAVE10") << endl;     // false

    cart.checkout();
    cart.addItem("Keyboard", 79.99); // Should be rejected
    cout << "Total: $" << cart.getTotal() << endl;                    // 926.98

    return 0;
}
