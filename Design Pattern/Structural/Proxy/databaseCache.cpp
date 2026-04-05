#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>

using namespace std;

class DatabaseService {
public:
  virtual string query(const string &sql) = 0;
  virtual ~DatabaseService() {}
};

class RealDatabaseService : public DatabaseService {
public:
  string query(const string &sql) override {
    cout << "RealDatabase: Executing query: " << sql << endl;
    this_thread::sleep_for(chrono::milliseconds(2000));
    return "Result for [" + sql + "]";
  }
};

class CachingDatabaseProxy : public DatabaseService {
private:
  RealDatabaseService realService;
  unordered_map<string, string> cache;

public:
  string query(const string &sql) override {
    auto it = cache.find(sql);
    if (it != cache.end()) {
      cout << "CachingProxy: Cache HIT for: " << sql << endl;
      return it->second;
    }
    cout << "CachingProxy: Cache MISS for: " << sql << endl;
    string result = realService.query(sql);
    cache[sql] = result;
    return result;
  }

  void clearCache() {
    cout << "CachingProxy: Cache cleared." << endl;
    cache.clear();
  }
};

int main() {
  CachingDatabaseProxy db;

  cout << "--- First query (cache miss) ---" << endl;
  cout << db.query("SELECT * FROM users") << endl;

  cout << "\n--- Same query again (cache hit) ---" << endl;
  cout << db.query("SELECT * FROM users") << endl;

  cout << "\n--- Different query (cache miss) ---" << endl;
  cout << db.query("SELECT * FROM orders WHERE status = 'pending'") << endl;

  cout << "\n--- Clear cache and retry ---" << endl;
  db.clearCache();
  cout << db.query("SELECT * FROM users") << endl;

  return 0;
}
