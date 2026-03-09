#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

// Before: One class doing three unrelated jobs
class ReportManager {
public:
    void createAndSendReport(const string& recipient) {
        // Responsibility 1: Generate report data
        vector<vector<string>> data = {
            {"Name", "Sales", "Region"},
            {"Alice", "15000", "North"},
            {"Bob", "22000", "South"},
            {"Charlie", "18000", "East"}
        };

        // Responsibility 2: Format as CSV
        stringstream csv;
        for (const auto& row : data) {
            for (size_t i = 0; i < row.size(); i++) {
                if (i > 0) csv << ",";
                csv << row[i];
            }
            csv << "\n";
        }

        // Responsibility 3: Distribute via email
        cout << "Sending report to: " << recipient << endl;
        cout << csv.str();
        cout << "Report sent successfully." << endl;
    }
};

// TODO: Refactor into ReportGenerator, ReportFormatter, and ReportDistributor.
class ReportGenerator {
public:
    vector<vector<string>> generate() const {
        return {
            {"Name", "Sales", "Region"},
            {"Alice", "15000", "North"},
            {"Bob", "22000", "South"},
            {"Charlie", "18000", "East"}
        };
    }
};

class ReportFormatter {
public:
    string formatAsCsv(const vector<vector<string>>& data) const {
        stringstream csv;
        for (const auto& row : data) {
            for (size_t i = 0; i < row.size(); i++) {
                if (i > 0) csv << ",";
                csv << row[i];
            }
            csv << "\n";
        }

        return csv.str();
    }
};

class ReportDistributor {
public:
    void distribute(const string& recipient, const string& csv) const {
        cout << "Sending report to: " << recipient << endl;
        cout << csv;
        cout << "Report sent successfully." << endl;
    }
};

int main() {
    // After refactoring, usage should look like:
    ReportGenerator generator;
    ReportFormatter formatter;
    ReportDistributor distributor;
    auto data = generator.generate();
    auto formatted = formatter.formatAsCsv(data);
    distributor.distribute("manager@company.com", formatted);
    return 0;
}
