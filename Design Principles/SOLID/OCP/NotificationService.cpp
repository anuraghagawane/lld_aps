#include <iostream>
#include <string>

using namespace std;

// class NotificationService {
// public:
//     void sendNotification(const string& channel, const string& message) {
//         if (channel == "Email") {
//             cout << "Sending EMAIL: " << message << endl;
//             // Email-specific logic
//         } else if (channel == "SMS") {
//             cout << "Sending SMS: " << message << endl;
//             // SMS-specific logic
//         } else {
//             throw invalid_argument("Unknown channel: " + channel);
//         }
//     }
// };

// int main() {
//     NotificationService service;
//     service.sendNotification("Email", "Your order has shipped!");
//     service.sendNotification("SMS", "Your order has shipped!");
//     return 0;
// }

// TODO: Define a NotificationChannel interface with a send(message) method.
// TODO: Create EmailChannel, SMSChannel, PushChannel, and SlackChannel.
// TODO: Refactor NotificationService to accept a NotificationChannel.

class NotificationChannel {
public:
  virtual ~NotificationChannel() = default;
  virtual void send(const string &message) = 0;
};

class EmailChannel : public NotificationChannel {
public:
  void send(const string &message) {
    cout << "Sending EMAIL: " << message << endl;
  }
};

class SMSChannel : public NotificationChannel {
public:
  void send(const string &message) {
    cout << "Sending SMS: " << message << endl;
  }
};

class PushChannel : public NotificationChannel {
public:
  void send(const string &message) {
    cout << "Sending PUSH: " << message << endl;
  }
};

class SlackChannel : public NotificationChannel {
public:
  void send(const string &message) {
    cout << "Sending SLACK: " << message << endl;
  }
};
class NotificationService {
public:
  void sendNotification(NotificationChannel &channel, const string &message) {
    channel.send(message);
  }
};

int main() {
  NotificationService service;
  EmailChannel email;
  service.sendNotification(email, "Your order has shipped!");

  SMSChannel sms;
  service.sendNotification(sms, "Your order has shipped!");

  PushChannel push;
  service.sendNotification(push, "Your order has shipped!");

  SlackChannel slack;
  service.sendNotification(slack, "Your order has shipped!");

  return 0;
}
