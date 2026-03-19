#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Email {
  string to, subject, body, priority;
  vector<string> cc, bcc, attachments;

public:
  void print() const {
    auto join = [](const vector<string> &v) {
      string result;
      for (size_t i = 0; i < v.size(); i++) {
        if (i > 0)
          result += ", ";
        result += v[i];
      }
      return result;
    };
    cout << "Email{to='" << to << "', subject='" << subject << "', cc=["
         << join(cc) << "], bcc=[" << join(bcc) << "], body='" << body
         << "', priority='" << priority << "', attachments=["
         << join(attachments) << "]}" << endl;
  }

  class Builder {
    string to, subject, body;
    string priority = "normal";
    vector<string> cc, bcc, attachments;

    friend class Email;

  public:
    Builder(const string &to, const string &subject)
        : to(to), subject(subject) {}

    Builder &addCc(const string &val) {
      cc.push_back(val);
      return *this;
    }

    Builder &addBcc(const string &val) {
      bcc.push_back(val);
      return *this;
    }

    Builder &setBody(const string &val) {
      body = val;
      return *this;
    }

    Builder &setPriority(const string &val) {
      priority = val;
      return *this;
    }

    Builder &addAttachment(const string &val) {
      attachments.push_back(val);
      return *this;
    }

    Email build() const {
      Email email;
      email.to = to;
      email.subject = subject;
      email.cc = cc;
      email.bcc = bcc;
      email.body = body;
      email.priority = priority;
      email.attachments = attachments;
      return email;
    }
  };
};

int main() {
  Email email1 = Email::Builder("alice@example.com", "Meeting Tomorrow")
                     .setBody("Let's meet at 10am in conference room B.")
                     .build();

  Email email2 = Email::Builder("bob@example.com", "Project Update")
                     .addCc("carol@example.com")
                     .addCc("dave@example.com")
                     .addBcc("manager@example.com")
                     .setBody("Attached is the Q4 report.")
                     .setPriority("high")
                     .addAttachment("q4-report.pdf")
                     .addAttachment("summary.xlsx")
                     .build();

  email1.print();
  cout << endl;
  email2.print();

  return 0;
}
