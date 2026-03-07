#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

class DisplayNameFormatter {
public:
    string formatDisplayName(const string& name) {
        // Your implementation here
        size_t start = name.find_first_not_of(' ');
        if(start == string::npos) return "";

        size_t end = name.find_last_not_of(' ');
        string trimmed = name.substr(start, end - start + 1);

        transform(trimmed.begin(), trimmed.end(), trimmed.begin(), ::tolower);
        trimmed[0] = toupper(trimmed[0]);
        return trimmed;
    }
};

int main() {
    DisplayNameFormatter formatter;
    cout << formatter.formatDisplayName("  john doe  ") << endl;
    cout << formatter.formatDisplayName("ALICE") << endl;
    cout << formatter.formatDisplayName("  bob  ") << endl;
    return 0;
}
