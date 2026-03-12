#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Before: Fat interface bundles three unrelated sets of operations
// class UserService {
// public:
//     virtual void createUser(const string& name, const string& email) = 0;
//     virtual string getUser(const string& userId) = 0;
//     virtual void updateUser(const string& userId, const string& newEmail) =
//     0; virtual void deleteUser(const string& userId) = 0;

//     virtual void banUser(const string& userId, const string& reason) = 0;
//     virtual void promoteUser(const string& userId, const string& role) = 0;

//     virtual vector<string> getLoginHistory(const string& userId) = 0;
//     virtual vector<string> getActivityLog(const string& userId) = 0;

//     virtual ~UserService() = default;
// };

// class BasicUserService : public UserService {
// public:
//     void createUser(const string& name, const string& email) override {
//         cout << "Creating user: " << name << " (" << email << ")" << endl;
//     }

//     string getUser(const string& userId) override {
//         cout << "Fetching user: " << userId << endl;
//         return "User-" + userId;
//     }

//     void updateUser(const string& userId, const string& newEmail) override {
//         cout << "Updating user " << userId << " email to " << newEmail <<
//         endl;
//     }

//     void deleteUser(const string& userId) override {
//         cout << "Deleting user: " << userId << endl;
//     }

//     void banUser(const string& /*userId*/, const string& /*reason*/) override
//     {
//         throw runtime_error("Not an admin service.");
//     }

//     void promoteUser(const string& /*userId*/, const string& /*role*/)
//     override {
//         throw runtime_error("Not an admin service.");
//     }

//     vector<string> getLoginHistory(const string& /*userId*/) override {
//         throw runtime_error("No audit capability.");
//     }

//     vector<string> getActivityLog(const string& /*userId*/) override {
//         throw runtime_error("No audit capability.");
//     }
// };

// TODO: Create UserCrud, AdminControls, and AuditLog interfaces.
// TODO: Refactor BasicUserService to implement only UserCrud.
// TODO: Create an AdminUserService that implements UserCrud and AdminControls.
// TODO: Create a FullUserService that implements all three interfaces.

class UserCrud {
public:
  virtual void createUser(const string &name, const string &email) = 0;
  virtual string getUser(const string &userId) = 0;
  virtual void updateUser(const string &userId, const string &newEmail) = 0;
  virtual void deleteUser(const string &userId) = 0;
  virtual ~UserCrud() = default;
};

class AdminControls {
public:
  virtual void banUser(const string &userId, const string &reason) = 0;
  virtual void promoteUser(const string &userId, const string &role) = 0;
  virtual ~AdminControls() = default;
};

class AuditLog {
public:
  virtual vector<string> getLoginHistory(const string &userId) = 0;
  virtual vector<string> getActivityLog(const string &userId) = 0;
  virtual ~AuditLog() = default;
};

class BasicUserService : public UserCrud {
public:
  void createUser(const string &name, const string &email) override {
    cout << "BasicUserService -> Creating user: " << name << " (" << email
         << ")" << endl;
  }

  string getUser(const string &userId) override {
    cout << "BasicUserService -> Fetching user: " << userId << endl;
    return "User-" + userId;
  }

  void updateUser(const string &userId, const string &newEmail) override {
    cout << "BasicUserService -> Updating user " << userId << " email to "
         << newEmail << endl;
  }

  void deleteUser(const string &userId) override {
    cout << "BasicUserService -> Deleting user: " << userId << endl;
  }
};

class AdminUserService : public UserCrud, public AdminControls {
public:
  void createUser(const string &name, const string &email) override {
    cout << "AdminUserService -> Creating user: " << name << " (" << email
         << ")" << endl;
  }

  string getUser(const string &userId) override {
    cout << "AdminUserService -> Fetching user: " << userId << endl;
    return "User-" + userId;
  }

  void updateUser(const string &userId, const string &newEmail) override {
    cout << "AdminUserService -> Updating user " << userId << " email to "
         << newEmail << endl;
  }

  void deleteUser(const string &userId) override {
    cout << "AdminUserService -> Deleting user: " << userId << endl;
  }

  void banUser(const string &userId, const string &reason) override {
    cout << "AdminUserService -> Banning user " << userId << ": " << reason
         << endl;
  }

  void promoteUser(const string &userId, const string &role) override {
    cout << "AdminUserService -> Promoting user " << userId << " to " << role
         << endl;
  }
};

class FullUserService : public UserCrud, public AdminControls, public AuditLog {
public:
  void createUser(const string &name, const string &email) override {
    cout << "FullUserService -> Creating user: " << name << " (" << email << ")"
         << endl;
  }

  string getUser(const string &userId) override {
    cout << "FullUserService -> Fetching user: " << userId << endl;
    return "User-" + userId;
  }

  void updateUser(const string &userId, const string &newEmail) override {
    cout << "FullUserService -> Updating user " << userId << " email to "
         << newEmail << endl;
  }

  void deleteUser(const string &userId) override {
    cout << "FullUserService -> Deleting user: " << userId << endl;
  }

  void banUser(const string &userId, const string &reason) override {
    cout << "FullUserService -> Banning user " << userId << ": " << reason
         << endl;
  }

  void promoteUser(const string &userId, const string &role) override {
    cout << "FullUserService -> Promoting user " << userId << " to " << role
         << endl;
  }

  vector<string> getLoginHistory(const string &userId) override {
    vector<string> history = {"2024-01-01", "2024-01-05"};
    cout << "FullUserService -> Login history for " << userId
         << ": [2024-01-01, 2024-01-05]" << endl;
    return history;
  }

  vector<string> getActivityLog(const string &userId) override {
    vector<string> log = {"created_post", "updated_profile"};
    cout << "FullUserService -> Activity log for " << userId
         << ": [created_post, updated_profile]" << endl;
    return log;
  }
};

int main() {
  BasicUserService svc;
  svc.createUser("Alice", "alice@example.com");
  svc.getUser("u123");

  AdminUserService aus;
  aus.createUser("Bob", "bob@example.com");
  aus.banUser("u456", "spam");
  aus.promoteUser("u456", "admin");

  FullUserService fus;
  fus.createUser("Carol", "carol@example.com");
  fus.banUser("u789", "abuse");
  fus.getLoginHistory("u789");
  return 0;
}
