#include <iostream>
#include <string>

using namespace std;

class User {
private:
    string username;
    string email;
    string password;

public:
    User(const string& username, const string& email, const string& password)
        : username(username), email(email), password(password) {}

    string getUsername() const {
        return username;
    }

    string getEmail() const {
        return email;
    }

    string getPassword() const {
        return password;
    }
};

class PasswordHasher {
public:
    string validateAndHash(const string& password) {
        if (password.length() < 8) {
            throw invalid_argument("Password must be at least 8 characters");
        }
        // Generate salt and hash with bcrypt
        return "bcrypt_hashed_" + password; // Simplified for illustration
    }
};

class UserRepository {
public:
    void save(const User& user) {
        cout << "Saving user " << user.getUsername() << " to database..." << endl;
    }
};

class AuthTokenService {
public:
    string generateToken(const User& user) {
        // Create JWT payload with user claims
        string payload = "{\"username\":\"" + user.getUsername() + "\",\"email\":\"" + user.getEmail() + "\"}";
        // Sign with secret key (simplified)
        return "eyJhbGciOiJIUzI1NiJ9." + payload + ".signature";
    }
};

class EmailService {
public:
    void sendWelcomeEmail(const User& user) {
        cout << "Sending welcome email to: " << user.getEmail() << endl;
        cout << "Welcome to our platform, " << user.getUsername() << "!" << endl;
    }
};

class AuthService {
private:
    PasswordHasher passwordHasher;
    UserRepository userRepository;
    AuthTokenService tokenService;

public:
    string registerUser(const string& username,
                        const string& email,
                        const string& password) {

        // 1. Validate & hash password
        string hashedPassword = passwordHasher.validateAndHash(password);

        // 2. Create user object
        User user(username, email, hashedPassword);

        // 3. Save user to database
        userRepository.save(user);

        // 4. Generate auth token
        string token = tokenService.generateToken(user);

        return token;
    }
};

int main() {
    AuthService authService;

    try {
        string token = authService.registerUser(
            "john_doe",
            "john@example.com",
            "strongpassword"
        );

        cout << "Generated Token: " << token << endl;
    }
    catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}
