#include <iostream>
#include <string>

using namespace std;

class PasswordValidator {
public:
    bool isValid(const string& password) {
        // Your implementation here
        return password.length() >= 8;
    }
};

int main() {
    PasswordValidator validator;
    cout << boolalpha;
    cout << validator.isValid("short") << endl;
    cout << validator.isValid("longenough") << endl;
    cout << validator.isValid("12345678") << endl;
    cout << validator.isValid("") << endl;
    return 0;
}
