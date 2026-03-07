#include <iostream>
#include <string>
#include <map>
#include <cstdlib>
#include <memory>
#include <vector>

using namespace std;

// Before: Each source has its own load-parse-validate pipeline
// class AppConfig {
// private:
//     map<string, string> fileConfig;
//     map<string, string> defaults;

// public:
//     AppConfig() {
//         fileConfig["db.host"] = "localhost";
//         fileConfig["db.port"] = "5432";
//         defaults["db.host"] = "127.0.0.1";
//         defaults["db.port"] = "3306";
//         defaults["db.timeout"] = "30";
//     }

//     string getFromFile(const string& key) {
//         auto it = fileConfig.find(key);
//         if (it == fileConfig.end() || it->second.empty()) return ""; // Duplicated
//         return it->second;
//     }

//     string getFromEnv(const string& key) {
//         const char* value = getenv(key.c_str());
//         if (value == nullptr || string(value).empty()) return ""; // Duplicated
//         return string(value);
//     }

//     string getFromDefaults(const string& key) {
//         auto it = defaults.find(key);
//         if (it == defaults.end() || it->second.empty()) return ""; // Duplicated
//         return it->second;
//     }
// };

// TODO: Extract a ConfigSource interface and create a ConfigLoader.
class ConfigSource {
public:
    virtual ~ConfigSource() = default;
    virtual string loadValue(const string& key) const = 0;
};

class FileConfigSource : public ConfigSource {
private:
    map<string, string> config;
public:
    FileConfigSource(const map<string, string>& config) 
        : config(config) {}

    string loadValue(const string& key) const override {
        auto it = config.find(key);
        if(it != config.end()) return it->second;
        return "";
    }
};

class EnvConfigSource : public ConfigSource {
public:
    string loadValue(const string& key) const override {
        string envKey = key;
        for(auto& c: envKey) {
            if(c == '.') c = '_';
            else c = toupper(c);
        }
        const char* value = getenv(envKey.c_str());
        return value ? string(value) : "";
    }
};

class DefaultConfigSource : public ConfigSource {
private:
    map<string, string> defaults;
public:
    DefaultConfigSource(const map<string, string>& defaults)
        : defaults(defaults) {}
    string loadValue(const string& key) const override {
        auto it = defaults.find(key);
        if(it != defaults.end()) return it->second;
        return "";
    }  
};

class ConfigLoader {
private:
    vector<unique_ptr<ConfigSource>> sources;
public:
    ConfigLoader(vector<unique_ptr<ConfigSource>> sources) 
        : sources(move(sources)) {}

    string get(const string& key) const {
        for(const auto& source: sources) {
            string value = source->loadValue(key);
            if(!value.empty()) return value;
        }
        return "";
    }
};

int main() {
    // After refactoring, usage should look like:
    // ConfigLoader loader({fileSource, envSource, defaultSource});
    // string host = loader.get("db.host");
    map<string, string> fileConfig = {
        {"db.host", "localhost"}, {"db.port", "5432"}
    };
    map<string, string> defaults = {
        {"db.host", "127.0.0.1"}, {"db.port", "3306"}, {"db.timeout", "30"}
    };

    vector<unique_ptr<ConfigSource>> sources;
    sources.push_back(make_unique<FileConfigSource>(fileConfig));
    sources.push_back(make_unique<EnvConfigSource>());
    sources.push_back(make_unique<DefaultConfigSource>(defaults));

    ConfigLoader loader(move(sources));

    cout << "db.host = " << loader.get("db.host") << endl;
    cout << "db.port = " << loader.get("db.port") << endl;
    cout << "db.timeout = " << loader.get("db.timeout") << endl;

    return 0;
}
