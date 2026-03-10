#include <iostream>
#include <string>

using namespace std;
class MessageFormatter {
public:
  static string format(const string &category, const string &userId,
                       const string &details) {
    string message = "[" + category + "] Hi " + userId + ", " + details;
    message[0] = toupper(message[0]);
    return message;
  }
};

class NotificationSender {
public:
  static void send(const string &userId, const string &message) {
    cout << "Connecting to notification API..." << endl;
    cout << "Sending to " << userId << ": " << message << endl;
    cout << "Notification sent successfully." << endl;
  }
};

class OrderService {
public:
  void notifyOrderConfirmation(const string &userId, const string &orderId) {
    string message = MessageFormatter::format(
        "Order", userId, "your order " + orderId + " has been confirmed.");
    NotificationSender::send(userId, message);
  }
};

class ShippingService {
public:
  void notifyShipmentUpdate(const string &userId, const string &trackingId) {
    string message = MessageFormatter::format(
        "Shipping", userId, "your shipment " + trackingId + " is on its way.");
    NotificationSender::send(userId, message);
  }
};

class SupportService {
public:
  void notifyTicketResolution(const string &userId, const string &ticketId) {
    string message = MessageFormatter::format(
        "Support", userId, "your ticket " + ticketId + " has been resolved.");
    NotificationSender::send(userId, message);
  }
};

int main() {
  OrderService os = OrderService();
  os.notifyOrderConfirmation("U_12", "O_12");

  ShippingService ss = ShippingService();
  ss.notifyShipmentUpdate("U_12", "O_12");

  SupportService sus = SupportService();
  sus.notifyTicketResolution("U_12", "O_12");
  return 0;
}
