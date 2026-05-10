#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <vector>

using namespace std;

enum class LogLevel { DEBUG, INFO, WARNING, ERROR, FATAL };

string logLevelToString(LogLevel level) {
  switch (level) {
  case LogLevel::DEBUG:
    return "DEBUG";
  case LogLevel::INFO:
    return "INFO";
  case LogLevel::WARNING:
    return "WARNING";
  case LogLevel::ERROR:
    return "ERROR";
  case LogLevel::FATAL:
    return "FATAL";
  }
  return "";
}

class LogMessage {
public:
  chrono::system_clock::time_point timestamp;
  LogLevel level;
  string message;

  LogMessage(LogLevel level, string message)
      : timestamp(chrono::system_clock::now()), level(level), message(message) {
  }
};

class LogFormatter {
public:
  virtual string format(const LogMessage &msg) = 0;
  virtual ~LogFormatter() = default;
};

class PlainTextFormatter : public LogFormatter {
public:
  string format(const LogMessage &msg) override {
    auto time = chrono::system_clock::to_time_t(msg.timestamp);
    stringstream ss;
    ss << put_time(localtime(&time), "%Y-%m-%d %H:%M:%S") << " ["
       << logLevelToString(msg.level) << "] " << msg.message;
    return ss.str();
  }
};

class JsonFormatter : public LogFormatter {
public:
  string format(const LogMessage &msg) override {
    auto time = chrono::system_clock::to_time_t(msg.timestamp);
    stringstream ss;
    ss << "{"
       << "\"timestamp\":\"" << put_time(localtime(&time), "%Y-%m-%d %H:%M:%S")
       << "\","
       << "\"level\":\"" << logLevelToString(msg.level) << "\","
       << "\"message\":\"" << msg.message << "\""
       << "}";
    return ss.str();
  }
};

class LogAppender {
protected:
  shared_ptr<LogFormatter> formatter;

public:
  LogAppender(shared_ptr<LogFormatter> formatter) : formatter(formatter) {}
  virtual void append(const LogMessage &msg) = 0;
  virtual ~LogAppender() = default;
};

class ConsoleAppender : public LogAppender {
private:
  mutex mtx;

public:
  ConsoleAppender(shared_ptr<LogFormatter> formatter)
      : LogAppender(formatter) {}
  void append(const LogMessage &msg) override {
    lock_guard<mutex> lock(mtx);
    cout << formatter->format(msg) << endl;
  }
};

class FileAppender : public LogAppender {
private:
  ofstream file;
  mutex mtx;

public:
  FileAppender(const string &filename, shared_ptr<LogFormatter> formatter)
      : LogAppender(formatter) {
    file.open(filename, ios::app);
  }
  void append(const LogMessage &msg) override {
    lock_guard<mutex> lock(mtx);
    file << formatter->format(msg) << endl;
  }

  ~FileAppender() {
    if (file.is_open())
      file.close();
  }
};

class LoggerConfig {
public:
  vector<shared_ptr<LogAppender>> appenders;
  LogLevel minLevel;

  LoggerConfig(LogLevel minLevel) : minLevel(minLevel) {}
};

class Logger {
private:
  LoggerConfig config;
  mutex mtx;

  Logger() : config(LogLevel::DEBUG) {}

public:
  static Logger &getInstance() {
    static Logger instance;
    return instance;
  }

  void setConfig(const LoggerConfig &cfg) {
    lock_guard<mutex> lock(mtx);
    config = cfg;
  }

  void log(LogLevel level, const string &message) {
    vector<shared_ptr<LogAppender>> appendersCopy;
    {
      lock_guard<mutex> lock(mtx);
      if (config.minLevel > level)
        return;

      appendersCopy = config.appenders;
    }

    LogMessage msg(level, message);
    for (auto &appender : appendersCopy) {
      appender->append(msg);
    }
  }

  void debug(const string &msg) { log(LogLevel::DEBUG, msg); }
  void info(const string &msg) { log(LogLevel::INFO, msg); }
  void warn(const string &msg) { log(LogLevel::WARNING, msg); }
  void error(const string &msg) { log(LogLevel::ERROR, msg); }
  void fatal(const string &msg) { log(LogLevel::FATAL, msg); }
};

int main() {
  auto plainFormatter = make_shared<PlainTextFormatter>();
  auto jsonFormatter = make_shared<JsonFormatter>();
  LoggerConfig config(LogLevel::INFO);
  config.appenders.push_back(make_shared<ConsoleAppender>(plainFormatter));
  config.appenders.push_back(make_shared<ConsoleAppender>(jsonFormatter));
  config.appenders.push_back(
      make_shared<FileAppender>("log.txt", plainFormatter));
  config.appenders.push_back(
      make_shared<FileAppender>("jsonlog.txt", jsonFormatter));
  Logger::getInstance().setConfig(config);
  Logger::getInstance().log(LogLevel::DEBUG, "Hello");
  Logger::getInstance().log(LogLevel::FATAL, "Hello");
  Logger::getInstance().debug("Debugging");
  Logger::getInstance().error("error at 1");
  return 0;
}
