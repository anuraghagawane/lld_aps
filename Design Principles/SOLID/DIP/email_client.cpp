#include <iostream>
#include <memory>

using namespace std;

class EmailClient {
public:
  virtual void sendEmail(const string &to, const string &subject,
                         const string &body) = 0;
  virtual ~EmailClient() = default;
};

class GmailClientImpl : public EmailClient {
public:
  void sendEmail(const string &to, const string &subject, const string &body) {
    cout << "Connecting to Gmail SMTP server..." << endl;
    cout << "Sending email via Gmail to: " << to << endl;
    cout << "Subject: " << subject << endl;
    cout << "Body: " << body << endl;
    cout << "Gmail email sent successfully!" << endl;
  }
};

class OutlookClientImpl : public EmailClient {
public:
  void sendEmail(const string &to, const string &subject,
                 const string &body) override {
    cout << "Connecting to Outlook Exchange server..." << endl;
    cout << "Sending email via Outlook to: " << to << endl;
    cout << "Subject: " << subject << endl;
    cout << "Body: " << body << endl;
    cout << "Outlook email sent successfully!" << endl;
  }
};

class EmailService {
private:
  shared_ptr<EmailClient> emailClient;

public:
  EmailService(shared_ptr<EmailClient> client) : emailClient(client) {}

  void sendWelcomeEmail(const string &userEmail, const string &userName) {
    string subject = "Welcome, " + userName + "!";
    string body = "Thanks for signing up to our awesome platform. We're glad "
                  "to have you!";
    emailClient->sendEmail(userEmail, subject, body);
  }

  void sendPasswordResetEmail(const string &userEmail) {
    string subject = "Reset Your Password";
    string body = "Please click the link below to reset your password...";
    emailClient->sendEmail(userEmail, subject, body);
  }
};

int main() {
  cout << "--- Using Gmail ---" << endl;
  shared_ptr<EmailClient> gmail = make_shared<GmailClientImpl>();
  EmailService gmailService(gmail);
  gmailService.sendWelcomeEmail("test@example.com", "Alice");

  cout << "\n--- Using Outlook ---" << endl;
  shared_ptr<EmailClient> outlook = make_shared<OutlookClientImpl>();
  EmailService outlookService(outlook);
  outlookService.sendWelcomeEmail("test@example.com", "Alice");
  return 0;
}
