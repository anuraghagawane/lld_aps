#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Product {
private:
    string name;
    double price;
public:
    Product(const string& name, double price) : name(name), price(price) {}
    string getName() const { return name; }
    double getPrice() const { return price; }
};

class Catalog {
private:
    vector<Product*> products;
public:
    void addProduct(Product* product) {
        // TODO: Add product to catalog
        products.push_back(product);
    }

    Product* findByName(const string& name) {
        // TODO: Find and return product by name, return nullptr if not found
        for(const auto& product: products) {
            if(product -> getName() == name) {
                return product;
            }
        }
        return nullptr;
    }

    int getProductCount() const { return products.size(); }
};

class Cart {
private:
    vector<Product*> items;
public:
    void addItem(Product* product) {
        // TODO: Add product to cart
        items.push_back(product);
    }

    void clearCart() {
        // TODO: Remove all items (don't destroy the products!)
        items.clear();
    }

    double getTotal() const {
        // TODO: Sum prices of all items
        double sum = 0;
        for(const auto& item: items) {
            sum += item -> getPrice();
        }
        return sum;
    }

    vector<Product*> getItems() const { return items; }
    int getItemCount() const { return items.size(); }
};

class Customer {
private:
    string name;
    Cart* cart;
public:
    Customer(const string& name, Cart* cart) : name(name), cart(cart) {}

    void checkout() {
        // TODO: Print cart items and total, then clear cart
        cout<<"Alice checking out:"<<endl;
        for(const auto& item: cart->getItems()) {
            cout<<"  - "<<item->getName()<<": $"<<item->getPrice()<<endl;
        }
        cout<<"  Total: $"<<cart->getTotal()<<endl;
        cart -> clearCart();
    }

    string getName() const { return name; }
    Cart* getCart() const { return cart; }
};

int main() {
    Product laptop("Laptop", 999.99);
    Product mouse("Mouse", 29.99);
    Product keyboard("Keyboard", 79.99);

    Catalog catalog;
    catalog.addProduct(&laptop);
    catalog.addProduct(&mouse);
    catalog.addProduct(&keyboard);

    Cart cart1, cart2;
    Customer alice("Alice", &cart1);
    Customer bob("Bob", &cart2);

    cart1.addItem(&laptop);
    cart1.addItem(&mouse);
    cart2.addItem(&laptop);
    cart2.addItem(&keyboard);

    cout << "Alice's cart: " << cart1.getItemCount() << " items, $" << cart1.getTotal() << endl;
    cout << "Bob's cart: " << cart2.getItemCount() << " items, $" << cart2.getTotal() << endl;

    alice.checkout();

    cout << "Catalog still has " << catalog.getProductCount() << " products" << endl;
    cout << "Bob's cart still has " << cart2.getItemCount() << " items" << endl;
    cout << "Laptop still exists: " << laptop.getName() << endl;

    return 0;
}
