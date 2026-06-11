#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

enum class BookStatus { AVAILABLE, BORROWED };

enum class LoanStatus { ACTIVE, RETURNED };

class Book {
private:
  string id;
  string name;
  string author;
  string isbn;
  int publicationYear;
  BookStatus status;

public:
  Book(const string &id, const string &name, const string &author,
       const string &isbn, int publicationYear)
      : id(id), name(name), author(author), isbn(isbn),
        publicationYear(publicationYear), status(BookStatus::AVAILABLE) {}

  string getName() const { return name; }

  bool isAvailable() const { return status == BookStatus::AVAILABLE; }

  void borrow() { status = BookStatus::BORROWED; }

  void returnBook() { status = BookStatus::AVAILABLE; }
};

class Loan;

class Member {
private:
  string id;
  string name;
  vector<Loan *> loanHistory;

public:
  Member(const string &id, const string &name) : id(id), name(name) {}

  string getId() const { return id; }
  string getName() const { return name; }

  void addLoan(Loan *loan) { loanHistory.push_back(loan); }

  int getActiveLoanCount() const;
};

class Loan {
private:
  string id;
  Book *book;
  Member *member;
  chrono::system_clock::time_point borrowDate;
  chrono::system_clock::time_point dueDate;
  chrono::system_clock::time_point returnDate;
  LoanStatus status;

public:
  Loan(const string &id, Book *book, Member *member,
       chrono::system_clock::time_point borrowDate,
       chrono::system_clock::time_point dueDate)
      : id(id), book(book), member(member), borrowDate(borrowDate),
        dueDate(dueDate), status(LoanStatus::ACTIVE) {}

  void returnBook() {
    status = LoanStatus::RETURNED;
    returnDate = chrono::system_clock::now();
    book->returnBook();
  }

  LoanStatus getStatus() const { return status; }

  bool isBorrowed() const { return status == LoanStatus::ACTIVE; }
};

int Member::getActiveLoanCount() const {
  int count = 0;

  for (auto loan : loanHistory) {
    if (loan->isBorrowed()) {
      count++;
    }
  }

  return count;
}
class BorrowingPolicy {
private:
  int maxBooksAllowed;
  int loanDurationDays;

public:
  BorrowingPolicy(int maxBooksAllowed = 5, int loanDurationDays = 14)
      : maxBooksAllowed(maxBooksAllowed), loanDurationDays(loanDurationDays) {}

  int getMaxBooksAllowed() const { return maxBooksAllowed; }

  int getLoanDurationDays() const { return loanDurationDays; }

  bool canBorrow(const Member &member) const {
    return member.getActiveLoanCount() < maxBooksAllowed;
  }
};

class Library {
private:
  BorrowingPolicy policy;
  unordered_map<string, unique_ptr<Book>> books;
  unordered_map<string, unique_ptr<Member>> members;
  unordered_map<string, unique_ptr<Loan>> loans;

  mutable mutex mtx;

  atomic<int> nextBookId{1};
  atomic<int> nextMemberId{1};
  atomic<int> nextLoanId{1};

public:
  string registerMember(const string &name) {
    lock_guard<mutex> lock(mtx);
    string memid = "MEM_" + to_string(nextMemberId.fetch_add(1));
    cout << memid << endl;
    auto member = make_unique<Member>(memid, name);

    cout << "New Member: " << member->getName() << " ID: " << member->getId()
         << endl;

    members[memid] = std::move(member);
    return memid;
  }

  string addBook(const string &name, const string &author, const string &isbn,
                 int publicationYear) {
    lock_guard<mutex> lock(mtx);
    string bookId = "BOOK_" + to_string(nextBookId.fetch_add(1));
    auto book = make_unique<Book>(bookId, name, author, isbn, publicationYear);

    cout << "Book: " << book->getName() << " Added with ID: " << bookId << endl;

    books[bookId] = std::move(book);
    return bookId;
  }

  void removeBook(const string &bookId) {
    lock_guard<mutex> lock(mtx);
    auto it = books.find(bookId);

    if (it == books.end())
      throw runtime_error("Book not found");

    if (!it->second->isAvailable())
      throw runtime_error("Book currently borrowed");

    books.erase(it);
  }

  string borrowBook(const string &memberId, const string &bookId) {
    lock_guard<mutex> lock(mtx);
    auto book = books[bookId].get();
    if (!book || !book->isAvailable()) {
      throw runtime_error("No such book available");
    }

    auto member = members[memberId].get();
    if (!member) {
      throw runtime_error("Member not available");
    }

    if (!policy.canBorrow(*member)) {
      throw runtime_error("Already borrowed 5 books");
    }

    string borrowId = "LOAN_" + to_string(nextLoanId.fetch_add(1));
    auto currtime = chrono::system_clock::now();
    auto loan = make_unique<Loan>(
        borrowId, books[bookId].get(), members[memberId].get(), currtime,
        currtime + chrono::hours(policy.getLoanDurationDays() * 24));

    book->borrow();
    member->addLoan(loan.get());

    loans[borrowId] = std::move(loan);
    cout << "Member: " << member->getName() << " Borrowed " << book->getName()
         << endl;

    return borrowId;
  }

  void returnBook(const string &borrowId) {
    lock_guard<mutex> lock(mtx);
    auto loan = loans[borrowId].get();
    loan->returnBook();
    cout << "Return book with borrowId: " << borrowId << endl;
  }

  void showBookStatus() {
    for (auto &[id, book] : books) {
      cout << "ID: " << id << " Name: " << book->getName()
           << " Status: " << (book->isAvailable() ? "AVAILABLE" : "BORROWED")
           << endl;
    }
  }
};

int main() {
  auto library = make_unique<Library>();

  cout << "Library initiated" << endl;

  auto m1 = library->registerMember("Anurag");
  auto m2 = library->registerMember("Khali");

  auto b1 = library->addBook("title1", "auth1", "123", 2021);
  auto b2 = library->addBook("title2", "auth2", "223", 2023);

  auto l1 = library->borrowBook(m1, b1);
  library->showBookStatus();
  library->returnBook(l1);
  library->showBookStatus();
  return 0;
}
