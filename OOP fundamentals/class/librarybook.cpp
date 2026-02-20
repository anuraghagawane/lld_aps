#include <iostream>
#include <string>
using namespace std;

class Book {
private:
    // Add private fields: title (string), author (string), isbn (string), isAvailable (bool)
    string title;
    string author;
    string isbn;
    bool isAvailable;

public:
    Book(const string& title, const string& author, const string& isbn) {
        // Initialize fields. Book starts as available.
        this -> title = title;
        this -> author = author;
        this -> isbn = isbn;
        this -> isAvailable = true;
    }

    bool borrowBook() {
        // Mark book as unavailable if currently available
        // Return true if successful, false if already borrowed
        if(isAvailable) {
            isAvailable = false;
            return true;
        }
        return false;
    }

    void returnBook() {
        // Mark book as available
        isAvailable = true;
    }

    void displayInfo() const {
        // Print: "Title by Author (ISBN: isbn) - Available" or "- Borrowed"
        string status = isAvailable ? "Available" : "Borrowed";
        cout<<title<< " by "<<author<<" (ISBN: "<<isbn<<") - "<<status<<endl;
    }
};

int main() {
    Book book("The Pragmatic Programmer", "David Thomas", "978-0135957059");
    book.displayInfo();

    bool success = book.borrowBook();
    cout << "Borrow successful: " << boolalpha << success << endl;
    book.displayInfo();

    success = book.borrowBook();
    cout << "Borrow successful: " << success << endl;

    book.returnBook();
    book.displayInfo();

    return 0;
}
