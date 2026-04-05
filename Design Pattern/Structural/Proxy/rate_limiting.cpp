#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class ApiService {
public:
  virtual string request(const string &endpoint) = 0;
  virtual ~ApiService() {}
};

class RealApiService : public ApiService {
public:
  string request(const string &endpoint) override {
    return "Response from " + endpoint;
  }
};

class RateLimitingProxy : public ApiService {
  // TODO: Add a field to store a pointer to the real ApiService
  // TODO: Add a vector of chrono::steady_clock::time_point to store timestamps
  // TODO: Add a field for MAX_REQUESTS (3)
  // TODO: Add a field for TIME_WINDOW (10 seconds)
  ApiService *realService;
  vector<chrono::steady_clock::time_point> timestamps;
  static const int MAX_REQUESTS = 3;
  chrono::seconds TIME_WINDOW = chrono::seconds(10);

public:
  RateLimitingProxy(ApiService *service) {
    // TODO: Store the real service pointer
    // TODO: Set MAX_REQUESTS and TIME_WINDOW
    this->realService = service;
  }

  string request(const string &endpoint) override {
    // TODO: Get the current time using chrono::steady_clock::now()
    // TODO: Remove timestamps older than TIME_WINDOW from the vector
    // TODO: If the number of timestamps >= MAX_REQUESTS, return "Rate limit
    // exceeded. Try again later."
    // TODO: Add the current timestamp to the vector
    // TODO: Delegate to the real service's request() and return the result
    auto now = chrono::steady_clock::now();
    timestamps.erase(
        remove_if(timestamps.begin(), timestamps.end(),
                  [&](const auto &t) { return now - t > TIME_WINDOW; }),
        timestamps.end());
    if ((int)timestamps.size() >= MAX_REQUESTS) {
      return "Rate limit exceeded. Try again later.";
    }
    timestamps.push_back(now);
    return realService->request(endpoint);
  }
};

int main() {
  RealApiService real; // Replace with RateLimitingProxy
  RateLimitingProxy api(&real);

  cout << api.request("/users") << endl;
  cout << api.request("/orders") << endl;
  cout << api.request("/products") << endl;
  cout << api.request("/inventory") << endl; // Should be rejected
  return 0;
}
