#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class ReportExporter {
public:
    string exportCsv(const vector<vector<string>>& rows) {
        // Your implementation here
        string csvstring = "";
        for(const auto& row: rows) {
            for(const auto& entry: row) {
                csvstring += entry + ",";
            }
            csvstring.pop_back();
            csvstring += '\n';
        }
        return csvstring;
    }
};

int main() {
    ReportExporter exporter;
    vector<vector<string>> data = {
        {"Name", "Age", "City"},
        {"Alice", "30", "New York"},
        {"Bob", "25", "London"}
    };
    cout << exporter.exportCsv(data) << endl;
    return 0;
}
