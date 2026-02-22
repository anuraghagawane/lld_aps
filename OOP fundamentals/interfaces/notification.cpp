#include<iostream>
#include<string>

class NotificationService {
public:
  virtual ~NotificationService() {}
  virtual void send(const std::string& recipient, const std::string& message) = 0;
};

class EmailNotifier : public NotificationService {
public:
  void send(const std::string& recipient, const std::string& message) override {
    std::cout<<"[Email] to: "<<recipient<<" | "<<message<<std::endl;
  }
};

class SlackNotifier : public NotificationService {
public:
  void send(const std::string& recipient, const std::string& message) override {
    std::cout<<"[SLACK] Channel: "<<recipient<<" | "<<message<<std::endl;
  }
};

class WebhookNotifier : public NotificationService {
public:
  void send(const std::string& recipient, const std::string& message) override {
    std::cout<<"[Webhook] URL: "<<recipient<<" | "<<message<<std::endl;
  }
};

class AlertService {
private:
  NotificationService* notifier;

public:
  AlertService(NotificationService* notifier): notifier(notifier) {}

  void triggerAlert(const std::string& recipient, const std::string& issue) {
    std::string alertMessage = "ALERT: " + issue;
    notifier->send(recipient, alertMessage);
  }
};

int main() {
  EmailNotifier emailNotifier;

  AlertService emailAlerts(&emailNotifier);
  emailAlerts.triggerAlert("ops@company.com", "CPU usage at 95%");

  SlackNotifier slackNotifier;
  AlertService slackAlerts(&slackNotifier);
  slackAlerts.triggerAlert("#incidents", "Database connection pool exhausted");

  WebhookNotifier WebhookNotifier;
  AlertService webhookAlerts(&WebhookNotifier);
  webhookAlerts.triggerAlert("https://hooks.example.com/alerts", "Disk usage at 90%");
}

