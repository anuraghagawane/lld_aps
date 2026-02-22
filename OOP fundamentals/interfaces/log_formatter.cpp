#include <iostream>
#include <string>
using namespace std;

class Formatter {
public:
    virtual ~Formatter() {}
    virtual string format(const string& message) = 0;
};

class PlainFormatter : public Formatter {
public:
    string format(const string& message) override {
        // Return the message as-is
        return message;
    }
};

class JsonFormatter : public Formatter {
public:
    string format(const string& message) override {
        // Return the message wrapped in JSON: {"log": "message"}
        return "{\"log\": \"" + message + "\"}";
    }
};

class Logger {
    Formatter* formatter;
public:
    Logger(Formatter* formatter) : formatter(formatter) {}

    void log(const string& message) {
        // Use the formatter to format the message, then print the result
        cout<<formatter->format(message)<<endl;;
    }
};

int main() {
    PlainFormatter plain;
    Logger plainLogger(&plain);
    plainLogger.log("Server started on port 8080");

    JsonFormatter json;
    Logger jsonLogger(&json);
    jsonLogger.log("Server started on port 8080");

    return 0;
}
