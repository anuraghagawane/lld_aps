#include <iostream>
#include <string>
using namespace std;

class MessageSender {
public:
  virtual ~MessageSender() {}
  virtual void sendMessage(const string &content) = 0;
};

class EmailSender : public MessageSender {
public:
  void sendMessage(const string &content) override {
    cout << "Email: " << content << endl;
  }
};

class SmsSender : public MessageSender {
public:
  void sendMessage(const string &content) override {
    cout << "SMS: " << content << endl;
  }
};

class Message {
protected:
  // TODO: Add a MessageSender* field
  // TODO: Add a string field for the content
  MessageSender *sender;
  string content;

public:
  Message(MessageSender *sender, const string &content) {
    // TODO: Store the sender pointer and content
    this->sender = sender;
    this->content = content;
  }

  virtual ~Message() {}

  virtual void send() = 0;
};

class TextMessage : public Message {
public:
  TextMessage(MessageSender *sender, const string &content)
      : Message(sender, content) {}

  void send() override {
    // TODO: Delegate to sender->sendMessage() with the content as-is
    sender->sendMessage(content);
  }
};

class UrgentMessage : public Message {
public:
  UrgentMessage(MessageSender *sender, const string &content)
      : Message(sender, content) {}

  void send() override {
    // TODO: Delegate to sender->sendMessage() with "[URGENT] " prepended to the
    // content
    sender->sendMessage("[URGENT] " + content);
  }
};

int main() {
  EmailSender email;
  SmsSender sms;
  TextMessage m1(&email, "Hello there");
  UrgentMessage m2(&sms, "Server is down");
  m1.send();
  m2.send();
  return 0;
}
