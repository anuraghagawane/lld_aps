#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

class Team;

class Employee {
private:
    string name;
    string role;
    vector<Team*> teams;
public:
    Employee(const string& name, const string& role) : name(name), role(role) {}

    void addTeam(Team* team) {
        // TODO: Add team to employee's team list
        teams.push_back(team);
    }

    void removeTeam(Team* team) {
        // TODO: Remove team from employee's team list
        teams.erase(remove(teams.begin(), teams.end(), team), teams.end());

    }

    vector<string> getTeamNames() const;

    string getName() const { return name; }
    string getRole() const { return role; }
};

class Team {
private:
    string name;
    vector<Employee*> members;
public:
    Team(const string& name) : name(name) {}

    void addMember(Employee* employee) {
        // TODO: Add employee to team and register this team on the employee
        members.push_back(employee);
        employee->addTeam(this);
    }

    void dissolve() {
        // TODO: Remove all members, don't destroy employees
        for(auto* e: members) {
            e -> removeTeam(this);
        }
        members.clear();
    }

    string getName() const { return name; }
    vector<Employee*> getMembers() const { return members; }
    int getMemberCount() const { return members.size(); }
};

vector<string> Employee::getTeamNames() const {
    // TODO: Return list of team names
    vector<string> teamNames;
    for(const auto* team: teams){
        teamNames.push_back(team->getName());
    } 
    return teamNames;
}

class Company {
private:
    string name;
    vector<Employee*> employees;
    vector<Team*> teams;
public:
    Company(const string& name) : name(name) {}

    void addEmployee(Employee* employee) {
        // TODO: Add employee to company
        employees.push_back(employee);
    }

    void addTeam(Team* team) {
        // TODO: Add team to company
        teams.push_back(team);
    }

    void dissolveTeam(Team* team) {
        // TODO: Dissolve the team and remove it from company's team list
        team->dissolve();
        teams.erase(remove(teams.begin(), teams.end(), team), teams.end());
    }

    string getName() const { return name; }
    int getEmployeeCount() const { return employees.size(); }
    int getTeamCount() const { return teams.size(); }
};

int main() {
    Company company("TechCorp");

    Employee alice("Alice", "Engineer");
    Employee bob("Bob", "Designer");
    Employee charlie("Charlie", "Engineer");

    company.addEmployee(&alice);
    company.addEmployee(&bob);
    company.addEmployee(&charlie);

    Team backend("Backend");
    Team frontend("Frontend");

    company.addTeam(&backend);
    company.addTeam(&frontend);

    backend.addMember(&alice);
    backend.addMember(&charlie);
    frontend.addMember(&alice);
    frontend.addMember(&bob);

    cout << "Before dissolving:" << endl;
    auto aliceTeams = alice.getTeamNames();
    cout << "  " << alice.getName() << "'s teams: [";
    for (size_t i = 0; i < aliceTeams.size(); i++) {
        if (i > 0) cout << ", ";
        cout << aliceTeams[i];
    }
    cout << "]" << endl;
    cout << "  Backend has " << backend.getMemberCount() << " members" << endl;
    cout << "  Company has " << company.getTeamCount() << " teams, "
         << company.getEmployeeCount() << " employees" << endl;

    company.dissolveTeam(&backend);

    cout << "\nAfter dissolving Backend:" << endl;
    aliceTeams = alice.getTeamNames();
    cout << "  " << alice.getName() << "'s teams: [";
    for (size_t i = 0; i < aliceTeams.size(); i++) {
        if (i > 0) cout << ", ";
        cout << aliceTeams[i];
    }
    cout << "]" << endl;

    auto charlieTeams = charlie.getTeamNames();
    cout << "  " << charlie.getName() << "'s teams: [";
    for (size_t i = 0; i < charlieTeams.size(); i++) {
        if (i > 0) cout << ", ";
        cout << charlieTeams[i];
    }
    cout << "]" << endl;

    cout << "  Company has " << company.getTeamCount() << " teams, "
         << company.getEmployeeCount() << " employees" << endl;
    cout << "  " << alice.getName() << " still exists: " << alice.getRole() << endl;

    return 0;
}
