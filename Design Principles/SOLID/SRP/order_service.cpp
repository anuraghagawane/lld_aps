#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>

using namespace std;

// Before: One class doing three unrelated jobs
// class OrderService {
// private:
//     map<string, int> inventory;
//     vector<string> orders;

// public:
//     OrderService() {
//         inventory["LAPTOP"] = 10;
//         inventory["PHONE"] = 25;
//         inventory["TABLET"] = 15;
//     }

//     void placeOrder(const string& productId, int quantity,
//                     const string& customerEmail) {
//         // Responsibility 1: Inventory check
//         auto it = inventory.find(productId);
//         if (it == inventory.end() || it->second < quantity) {
//             cout << "Insufficient stock for " << productId << endl;
//             return;
//         }

//         // Responsibility 2: Order processing
//         double pricePerUnit = 100.0;
//         double total = pricePerUnit * quantity;
//         string orderId = "ORD-" + to_string(orders.size() + 1);
//         orders.push_back(orderId);

//         // Responsibility 3: Update inventory
//         it->second -= quantity;

//         // Responsibility 4: Send notification
//         cout << "Email to " << customerEmail << ": Order " << orderId
//                   << " confirmed. Total: $" << total << endl;
//     }
// };

// TODO: Refactor into OrderProcessor, InventoryManager, and NotificationService.
class InventoryManager {
private:
    map<string, int> inventory;
public:
    InventoryManager() {
        inventory["LAPTOP"] = 10;
        inventory["PHONE"] = 25;
        inventory["TABLET"] = 15;
    }

    bool isAvailable(const string& productId, int quantity) const {
        auto it = inventory.find(productId);

        if(it != inventory.end() && it->second >= quantity) {
            return true;
        }

        return false;
    }

    void update(const string& productId, int quantity) {
        auto it = inventory.find(productId);
        it->second -= quantity;
    }
};

class NotificationService {
public:
    void send(const string& customerEmail, const string& orderId, double total) const {
         cout << fixed << setprecision(1);
        cout << "Email to " << customerEmail << ": Order " << orderId
                  << " confirmed. Total: $" << total << endl;
    }
};

class OrderProcessor {
private:
    InventoryManager inventory;
    NotificationService notifications;
    vector<string> orders;
public:
    OrderProcessor(InventoryManager& inventory, NotificationService& notifications) 
        : inventory(inventory), notifications(notifications) {}
    
    void placeOrder(const string& productId, int quantity, const string& customerEmail) {
        if(!inventory.isAvailable(productId, quantity)) return;

        // Responsibility 2: Order processing
        double pricePerUnit = 100.0;
        double total = pricePerUnit * quantity;
        string orderId = "ORD-" + to_string(orders.size() + 1);
        orders.push_back(orderId);

        // Responsibility 3: Update inventory
        inventory.update(productId, quantity);

        // Responsibility 4: Send notification
        notifications.send(customerEmail, orderId, total);
    }
};

int main() {
    // After refactoring, usage should look like:
    InventoryManager inventory;
    NotificationService notifications;
    OrderProcessor processor(inventory, notifications);
    processor.placeOrder("LAPTOP", 2, "alice@example.com");
    return 0;
}
