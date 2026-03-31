#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class RecipientList {
public:
  vector<string> to;
  vector<string> cc;

  RecipientList(vector<string> to, vector<string> cc)
      : to(move(to)), cc(move(cc)) {}

  RecipientList deepCopy() const { return RecipientList(to, cc); }

  void addTo(const string &email) { to.push_back(email); }
  void addCc(const string &email) { cc.push_back(email); }
};

class EmailTemplate {
private:
  string subject;
  string body;
  RecipientList recipients;

public:
  EmailTemplate(string subject, string body, RecipientList recipients)
      : subject(move(subject)), body(move(body)), recipients(move(recipients)) {
  }

  EmailTemplate *clone() const {
    return new EmailTemplate(subject, body, recipients.deepCopy());
  }

  void setSubject(const string &s) { subject = s; }
  RecipientList &getRecipients() { return recipients; }

  void print() const {
    cout << "Email: " << subject << " | Recipients: {to=[";
    for (size_t i = 0; i < recipients.to.size(); i++) {
      if (i > 0)
        cout << ", ";
      cout << recipients.to[i];
    }
    cout << "], cc=[";
    for (size_t i = 0; i < recipients.cc.size(); i++) {
      if (i > 0)
        cout << ", ";
      cout << recipients.cc[i];
    }
    cout << "]}" << endl;
  }
};

int main() {
  RecipientList baseRecipients({"all@company.com"}, {"archive@company.com"});
  EmailTemplate baseTemplate(
      "Company Newsletter", "Monthly updates from the team...", baseRecipients);

  auto marketingEmail = unique_ptr<EmailTemplate>(baseTemplate.clone());
  marketingEmail->setSubject("Marketing Newsletter");
  marketingEmail->getRecipients().addTo("marketing@company.com");

  auto engineeringEmail = unique_ptr<EmailTemplate>(baseTemplate.clone());
  engineeringEmail->setSubject("Engineering Newsletter");
  engineeringEmail->getRecipients().addTo("eng-team@company.com");

  auto hrEmail = unique_ptr<EmailTemplate>(baseTemplate.clone());
  hrEmail->setSubject("HR Newsletter");
  hrEmail->getRecipients().addTo("hr@company.com");
  hrEmail->getRecipients().addCc("ceo@company.com");

  marketingEmail->print();
  engineeringEmail->print();
  hrEmail->print();

  cout << "\nBase template unchanged:" << endl;
  baseTemplate.print();

  return 0;
}
