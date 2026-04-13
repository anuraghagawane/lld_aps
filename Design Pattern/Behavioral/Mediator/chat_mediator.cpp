#include <iostream>
#include <string>
#include <vector>

using namespace std;

class User;

class ChatMediator {
public:
  virtual void sendMessage(const string &message, User *sender) = 0;
  virtual void addUser(User *user) = 0;
  virtual ~ChatMediator() {}
};

class User {
protected:
  ChatMediator *mediator;
  string name;

public:
  User(ChatMediator *mediator, const string &name)
      : mediator(mediator), name(name) {}

  virtual void send(const string &message) = 0;
  virtual void receive(const string &message, const string &from) = 0;
  string getName() const { return name; }
  virtual ~User() {}
};

class ChatRoom : public ChatMediator {
private:
  vector<User *> users;

public:
  void addUser(User *user) override {
    users.push_back(user);
    cout << user->getName() << " joined the chat." << endl;
  }

  void sendMessage(const string &message, User *sender) override {
    for (User *user : users) {
      if (user != sender) {
        user->receive(message, sender->getName());
      }
    }
  }
};

class ChatUser : public User {
public:
  ChatUser(ChatMediator *mediator, const string &name) : User(mediator, name) {}

  void send(const string &message) override {
    cout << name << " sends: " << message << endl;
    mediator->sendMessage(message, this);
  }

  void receive(const string &message, const string &from) override {
    cout << name << " receives from " << from << ": " << message << endl;
  }
};

int main() {
  ChatRoom chatRoom;

  ChatUser alice(&chatRoom, "Alice");
  ChatUser bob(&chatRoom, "Bob");
  ChatUser charlie(&chatRoom, "Charlie");

  chatRoom.addUser(&alice);
  chatRoom.addUser(&bob);
  chatRoom.addUser(&charlie);

  cout << endl;
  alice.send("Hey everyone!");
  cout << endl;
  bob.send("Hi Alice!");

  return 0;
}
