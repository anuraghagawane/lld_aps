#include <iostream>
#include <string>
#include <ctime>

using namespace std;

class Notification {
protected:
  string recipient;
  string message;
  string timestamp;

public:
  Notification(const string& recipient, const string& message) 
    : recipient(recipient), message(message) {
      time_t now = time(nullptr);
      char buf[20];
      strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
      timestamp = buf;
    }

  virtual ~Notification() {}

  string formatter() {
    return "[" + timestamp + "] To: " + recipient;
  }

  virtual void send() {
    cout<<formatter() <<endl;
    cout<<"Message: " <<message<<endl;
  }
};

class EmailNotification : public Notification {
private:
  string subject;

public:
  EmailNotification(const string& recipient, const string& message, const string& subject)
    : Notification(recipient, message), subject(subject) {}

  void send() override {
    cout<<formatter()<<endl;
    cout<<"Subject: "<<subject<<endl;
    cout<<"Body: "<<message<<endl;
    cout<<"Status: Email delivered" <<endl;
  }
};

class SMSNotification : public Notification {
private:
  string phoneNumber;
  static const int MAX_LENGTH = 160;

public:
  SMSNotification(const string& recipient, const string& message, const string& phoneNumber) 
    : Notification(recipient, message), phoneNumber(phoneNumber) {}

  void send() override {
    cout<<formatter()<<endl;
    cout<<"Phone: "<<phoneNumber<<endl;
    string smsBody = message.length() > MAX_LENGTH
      ? message.substr(0, MAX_LENGTH - 3) + "..."
      : message;

    cout<<"SMS: "<<smsBody<<endl;
    cout<<"Status: SMS send (" <<smsBody.length()<<"/"<<MAX_LENGTH<<" chars)"<<endl;
  }
};

class PushNotification : public Notification {
private:
  string deviceToken;
  string priority;

public:
  PushNotification(const string& recipient, const string& message, const string& deviceToken, const string& priority)
    : Notification(recipient, message), deviceToken(deviceToken), priority(priority) {}

  void send() override {
    cout<<formatter()<<endl;
    cout<<"Device: "<<deviceToken.substr(0, 8)<<"..."<<endl;
    cout<<"Priority: "<<priority<<endl;
    cout<<"Alert: "<<message<<endl;
    cout<<"Status: Push notification delivered" <<endl;
  }
};

int main() {
  EmailNotification email("alice@example.com", "Your order has been shipped!", "Order Update");
  email.send();
  cout<<endl;

  SMSNotification sms("Bob", "Your verification code is 482910", "+91 1234567890");
  sms.send();
  cout<<endl;

  PushNotification push("Charlie", "New message from Alice", "a6f0as6afads", "high");
  push.send();
  return 0;
}
