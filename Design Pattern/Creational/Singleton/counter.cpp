#include <iostream>
#include <mutex>

using namespace std;

class Counter {
  // TODO: Implement as Meyers' Singleton
  // Hint: static local variable in getInstance()

private:
  int count_ = 0;
  // TODO: Add mutex for thread safety
  mutex mutex_;

  Counter() = default;

public:
  Counter(const Counter &) = delete;
  Counter &operator=(const Counter &) = delete;
  static Counter &getInstance() {
    static Counter counter;
    return counter;
  }

  void increment() {
    // TODO: Make thread-safe
    lock_guard<mutex> lock(mutex_);
    count_++;
  }

  int getCount() {
    // TODO: Return current count
    lock_guard<mutex> lock(mutex_);
    return count_;
  }
};

int main() {
  // After implementing, usage should look like:
  Counter &c1 = Counter::getInstance();
  Counter &c2 = Counter::getInstance();
  cout << "Same instance: " << (&c1 == &c2 ? "true" : "false") << endl;
  for (int i = 0; i < 5; i++) {
    c1.increment();
  }
  cout << "Count after 5 increments: " << c1.getCount() << endl;
  return 0;
}
