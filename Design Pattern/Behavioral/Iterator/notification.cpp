#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Notification {
private:
  string message;
  string type;
  bool isReadFlag;

public:
  Notification(string msg, string t)
      : message(msg), type(t), isReadFlag(false) {}

  string getMessage() const { return message; }
  string getType() const { return type; }
  bool isRead() const { return isReadFlag; }
  void markRead() { isReadFlag = true; }
};

class NotificationCenter {
private:
  vector<Notification> notifications;

public:
  void add(const Notification &n) { notifications.push_back(n); }

  Notification &getAt(int index) { return notifications[index]; }

  int getSize() const { return notifications.size(); }
};

class AllNotificationsIterator {
private:
  NotificationCenter *center;
  int index;

public:
  AllNotificationsIterator(NotificationCenter *c) : center(c), index(0) {}

  bool hasNext() { return index < center->getSize(); }

  Notification &next() { return center->getAt(index++); }
};

class FilteredIterator {
private:
  NotificationCenter *center;
  string type;
  int index;

  void advanceToNext() {
    while (index < center->getSize() &&
           center->getAt(index).getType() != type) {
      index++;
    }
  }

public:
  FilteredIterator(NotificationCenter *c, string t)
      : center(c), type(t), index(0) {
    advanceToNext();
  }

  bool hasNext() { return index < center->getSize(); }

  Notification &next() {
    Notification &n = center->getAt(index);
    index++;
    advanceToNext();
    return n;
  }
};

class UnreadIterator {
private:
  NotificationCenter *center;
  int index;

  void advanceToNext() {
    while (index < center->getSize() && center->getAt(index).isRead()) {
      index++;
    }
  }

public:
  UnreadIterator(NotificationCenter *c) : center(c), index(0) {
    advanceToNext();
  }

  bool hasNext() { return index < center->getSize(); }

  Notification &next() {
    Notification &n = center->getAt(index);
    index++;
    advanceToNext();
    return n;
  }
};

int main() {
  NotificationCenter center;
  center.add(Notification("Your order shipped", "EMAIL"));
  center.add(Notification("Flash sale today!", "PUSH"));
  center.add(Notification("Verify your number", "SMS"));
  center.add(Notification("Invoice ready", "EMAIL"));
  center.add(Notification("New login detected", "PUSH"));

  center.getAt(0).markRead();
  center.getAt(2).markRead();

  cout << "--- All Notifications ---" << endl;
  AllNotificationsIterator all(&center);
  while (all.hasNext()) {
    cout << "  " << all.next().getMessage() << endl;
  }

  cout << "\n--- Email Only ---" << endl;
  FilteredIterator emails(&center, "EMAIL");
  while (emails.hasNext()) {
    cout << "  " << emails.next().getMessage() << endl;
  }

  cout << "\n--- Unread Only ---" << endl;
  UnreadIterator unread(&center);
  while (unread.hasNext()) {
    cout << "  " << unread.next().getMessage() << endl;
  }

  return 0;
}
