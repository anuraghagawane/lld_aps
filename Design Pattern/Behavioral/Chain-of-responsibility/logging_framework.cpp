#include <iostream>
#include <string>
using namespace std;

struct LogMessage {
  int level;
  string message;
  LogMessage(int level, string message) : level(level), message(message) {}
};

class Logger {
public:
  virtual void setNext(Logger *next) = 0;
  virtual void log(LogMessage msg) = 0;
  virtual ~Logger() {}
};

class BaseLogger : public Logger {
protected:
  Logger *next = nullptr;

public:
  void setNext(Logger *next) override { this->next = next; }
  void forward(LogMessage msg) {
    if (next)
      next->log(msg);
  }
};

class ErrorLogger : public BaseLogger {
public:
  void log(LogMessage msg) override {
    // TODO: If msg.level >= 4, print "[ERROR] " + msg.message
    if (msg.level >= 4) {
      cout << "[ERROR] " << msg.message << endl;
    }
    // TODO: Always forward to next
    forward(msg);
  }
};

class WarnLogger : public BaseLogger {
public:
  void log(LogMessage msg) override {
    // TODO: If msg.level >= 3, print "[WARN] " + msg.message
    // TODO: Always forward to next
    if (msg.level >= 3) {
      cout << "[WARN] " << msg.message << endl;
    }
    forward(msg);
  }
};

class InfoLogger : public BaseLogger {
public:
  void log(LogMessage msg) override {
    // TODO: If msg.level >= 2, print "[INFO] " + msg.message
    // TODO: Always forward to next
    if (msg.level >= 2) {
      cout << "[INFO] " << msg.message << endl;
    }
    forward(msg);
  }
};

class DebugLogger : public BaseLogger {
public:
  void log(LogMessage msg) override {
    // TODO: If msg.level >= 1, print "[DEBUG] " + msg.message
    // TODO: Always forward to next
    if (msg.level >= 1) {
      cout << "[DEBUG] " << msg.message << endl;
    }
    forward(msg);
  }
};

int main() {
  ErrorLogger error;
  WarnLogger warn;
  InfoLogger info;
  DebugLogger debug;
  error.setNext(&warn);
  warn.setNext(&info);
  info.setNext(&debug);
  error.log(LogMessage(2, "User logged in"));
  error.log(LogMessage(4, "Database connection lost"));
  return 0;
}
