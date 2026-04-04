#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class OrgComponent {
public:
  virtual int getSalary() = 0;
  virtual int getHeadcount() = 0;
  virtual void printHeirarchy(string indent) = 0;
  virtual ~OrgComponent() {}
};

class Employee : public OrgComponent {
private:
  string name, title;
  int salary;

public:
  Employee(string name, string title, int salary)
      : name(name), title(title), salary(salary) {}

  int getSalary() override { return salary; }

  int getHeadcount() override { return 1; }

  void printHeirarchy(string indent) override {
    cout << indent << "- " << name << " (" << title << ", $" << salary << ")"
         << endl;
  }
};

class Manager : public OrgComponent {
private:
  string name, title;
  int salary;
  vector<OrgComponent *> members;

public:
  Manager(string name, string title, int salary)
      : name(name), title(title), salary(salary) {}

  void addMember(OrgComponent *member) { members.push_back(member); }

  void removeMember(OrgComponent *member) {
    members.erase(remove(members.begin(), members.end(), member),
                  members.end());
  }

  int getSalary() override {
    int total = salary;
    for (OrgComponent *member : members)
      total += member->getSalary();
    return total;
  }

  int getHeadcount() override {
    int count = 1;
    for (OrgComponent *member : members)
      count += member->getHeadcount();
    return count;
  }

  void printHeirarchy(string indent) override {
    cout << indent << "+ " << name << " (" << title << ", $" << salary << ")"
         << endl;
    for (OrgComponent *member : members)
      member->printHeirarchy(indent + "  ");
  }
};

int main() {
  Employee dev1("Alice", "Senior Engineer", 120000);
  Employee dev2("Bob", "Engineer", 95000);
  Employee dev3("Charlie", "Engineer", 90000);
  Employee designer("Diana", "Designer", 100000);

  Manager techLead("Eve", "Tech Lead", 140000);
  techLead.addMember(&dev1);
  techLead.addMember(&dev2);

  Manager vpEng("Frank", "VP Engineering", 200000);
  vpEng.addMember(&techLead);
  vpEng.addMember(&dev3);

  Manager vpProduct("Grace", "VP Product", 190000);
  vpProduct.addMember(&designer);

  Manager ceo("Hank", "CEO", 300000);
  ceo.addMember(&vpEng);
  ceo.addMember(&vpProduct);

  cout << "---- Organization Chart ----" << endl;
  ceo.printHeirarchy("");

  cout << "\nTotal Payroll: $" << ceo.getSalary() << endl;
  cout << "Total Headcount: " << ceo.getHeadcount() << endl;
  cout << "\nEngineering Payroll: $" << vpEng.getSalary() << endl;
  cout << "Engineering Headcount: " << vpEng.getHeadcount() << endl;

  return 0;
}
