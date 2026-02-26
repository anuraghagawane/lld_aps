#include <iostream>
#include <string>
#include <vector>

using namespace std;

class DataExporter {
public:
    virtual ~DataExporter() {}

    bool validate(const vector<string>& data) {
        // Return false and print "Export failed: No data to export." if data is empty
        // Return true and print "Validation passed. Exporting N records." otherwise
        if(data.size() == 0) {
            cout<<"Export failed: No data to export."<<endl;
            return false;
        }
        cout<<"Validation passed. Exporting "<<data.size()<<" records."<<endl;
        return true;
    }

    virtual void exportData(const vector<string>& data) = 0;
};

class CSVExporter : public DataExporter {
public:
    void exportData(const vector<string>& data) override {
        // Call validate(data) first. If validation fails, return early.
        // Otherwise, print CSV format: "CSV: Alice,Bob,Charlie"
        if(!validate(data)) {
            return;
        }

        cout<<"CSV: ";
        for (size_t i = 0; i < data.size(); i++) {
            cout << data[i];
            if (i < data.size() - 1) cout << ",";
        }
        cout << endl;
    }
};

class JSONExporter : public DataExporter {
public:
    void exportData(const vector<string>& data) override {
        // Call validate(data) first. If validation fails, return early.
        // Otherwise, print JSON array format: JSON: ["Alice", "Bob", "Charlie"]
        if(!validate(data)) {
            return;
        }

        cout<<"JSON: [";
        for (size_t i = 0; i < data.size(); i++) {
            cout << "\"" << data[i] << "\"";
            if (i < data.size() - 1) cout << ", ";
        }
        cout << "]" << endl;
    }
};

int main() {
    CSVExporter csv;
    csv.exportData({"Alice", "Bob", "Charlie"});

    cout << endl;

    JSONExporter json;
    json.exportData({"Alice", "Bob", "Charlie"});

    cout << endl;

    csv.exportData({}); // Should fail validation
    return 0;
}
