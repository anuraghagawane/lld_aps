#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Book {
    string title;
    string author;
    bool available;
public:
    Book(string title, string author)
        : title(title), author(author), available(true) {}

    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool isAvailable() const { return available; }
    void markCheckedOut() { available = false; }
};

class Section {
    string name;
    vector<Book> books;
public:
    Section(string name) : name(name) {}

    void addBook(const Book& book) { books.push_back(book); }

    // TODO: Add a checkoutBook(const string& title) method that returns bool
    // - Search through this section's books for a matching title
    // - If found AND available, mark it as checked out and return true
    // - If not found or already checked out, return false
    bool checkoutBook(const string& title) {
        for(auto& book: books) {
            if(book.getTitle() == title && book.isAvailable()) {
                book.markCheckedOut();
                return true;
            }
        }
        return false;
    }
};

class Library {
    vector<Section> sections;
public:
    void addSection(const Section& section) { sections.push_back(section); }

    // TODO: Add a checkoutBook(const string& title) method that returns bool
    // - Iterate through all sections and delegate to each section's checkoutBook()
    // - Return true as soon as any section successfully checks out the book
    // - Return false if no section has the book or it's unavailable
    // - CheckoutService should only need to call this single method
    bool checkoutBook(const string& title) {
        for(auto& section: sections) {
            if(section.checkoutBook(title)) return true;
        }
        return false;
    }
};

class CheckoutService {
    Library* library;
public:
    CheckoutService(Library* library) : library(library) {}

    bool checkout(const string& bookTitle) {
        // TODO: Replace with a single call to library's checkoutBook() method
        // This method should be one line - no direct access to sections or books
        return library->checkoutBook(bookTitle);
    }
};

int main() {
    Library library;

    Section fiction("Fiction");
    fiction.addBook(Book("The Great Gatsby", "F. Scott Fitzgerald"));
    fiction.addBook(Book("1984", "George Orwell"));
    library.addSection(fiction);

    Section nonFiction("Non-Fiction");
    nonFiction.addBook(Book("Clean Code", "Robert C. Martin"));
    library.addSection(nonFiction);

    CheckoutService service(&library);

    bool result1 = service.checkout("The Great Gatsby");
    cout << "Checking out \"The Great Gatsby\"... " << (result1 ? "Success!" : "Failed (not found)") << endl;

    bool result2 = service.checkout("The Great Gatsby");
    cout << "Checking out \"The Great Gatsby\"... " << (result2 ? "Success!" : "Failed (already checked out)") << endl;

    bool result3 = service.checkout("Unknown Book");
    cout << "Checking out \"Unknown Book\"... " << (result3 ? "Success!" : "Failed (not found)") << endl;

    return 0;
}
