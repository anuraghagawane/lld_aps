#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Logger {
public:
    virtual ~Logger() {}
    virtual void log(const string& level, const string& message) = 0;
    virtual string getDestination() = 0;
};

class ConsoleLogger : public Logger {
public:
    void log(const string& level, const string& message) override {
        // TODO: print "[level] message" to console
        cout<<"["<<level<<"] "<< message<<endl;
    }

    string getDestination() override {
        // TODO: return "Console"
        return "Console";
    }
};

class FileLogger : public Logger {
private:
    string filePath;

public:
    FileLogger(const string& filePath) {
        // TODO: initialize this->filePath
        this -> filePath = filePath;
    }

    void log(const string& level, const string& message) override {
        // TODO: print "Writing to filePath: [level] message"
        cout<<"Writing to "<<filePath<<": "<<"["<<level<<"] "<< message<<endl;
    }

    string getDestination() override {
        // TODO: return "File: filePath"
        return "File: " + filePath;
    }
};

class DatabaseLogger : public Logger {
private:
    string tableName;

public:
    DatabaseLogger(const string& tableName) {
        // TODO: initialize this->tableName
        this -> tableName = tableName;
    }

    void log(const string& level, const string& message) override {
        // TODO: print "INSERT INTO tableName: [level] message"
        cout<<"INSERT INTO "<<tableName<<": "<<"["<<level<<"] "<< message<<endl;
    }

    string getDestination() override {
        // TODO: return "Database: tableName"
        return "Database: " + tableName;
    }
};

class Application {
private:
    Logger* logger;

public:
    Application(Logger* logger) {
        // TODO: initialize this->logger
        this -> logger = logger;
    }

    void run() {
        // TODO: log three messages with level "INFO":
        //   "Application starting..."
        //   "Processing data..."
        //   "Application shutting down."
        logger->log("INFO", "Application starting...");
         logger->log("INFO", "Processing data...");
          logger->log("INFO", "Application shutting down.");
    }
};

int main() {
    ConsoleLogger console;
    FileLogger file("/var/log/app.log");
    DatabaseLogger db("app_logs");

    vector<Logger*> loggers = {&console, &file, &db};

    for (auto* logger : loggers) {
        cout << "--- Using " << logger->getDestination() << " ---" << endl;
        Application app(logger);
        app.run();
        cout << endl;
    }

    return 0;
}
