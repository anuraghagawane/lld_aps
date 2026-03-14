#include <iostream>
#include <memory>
#include <string>

using namespace std;

// Before: WeatherApp is tightly coupled to OpenWeatherMapAPI
class OpenWeatherMapAPI {
public:
  string fetchWeather(const string &city) {
    cout << "Calling OpenWeatherMap API for: " << city << endl;
    return "Sunny, 25C";
  }
};

// class WeatherApp {
// private:
//   OpenWeatherMapAPI api; // Direct dependency!
//
// public:
//   void displayWeather(const string &city) {
//     string weather = api.fetchWeather(city);
//     cout << "Weather in " << city << ": " << weather << endl;
//   }
// };

// int main() {
//   WeatherApp app;
//   app.displayWeather("London");
//   return 0;
// }

// TODO: Create a WeatherProvider interface (abstract class).
// TODO: Refactor WeatherApp to accept a WeatherProvider via its constructor.

class WeatherProvider {
public:
  virtual string getWeather(const string &city) = 0;
};

class OpenWeatherMapAPIProvider : public WeatherProvider {
public:
  string getWeather(const string &city) override {
    cout << "Calling OpenWeatherMap API for: " << city << endl;
    return "Sunny, 25C";
  }
};

class WeatherStackProvider : public WeatherProvider {
public:
  string getWeather(const string &city) override {
    cout << "Calling WeatherStack API for: " << city << endl;
    return "Cloudy, 18C";
  }
};

class WeatherApp {
private:
  unique_ptr<WeatherProvider> api; // Direct dependency!

public:
  WeatherApp(unique_ptr<WeatherProvider> api) : api(std::move(api)) {}
  void displayWeather(const string &city) {
    string weather = api->getWeather(city);
    cout << "Weather in " << city << ": " << weather << endl;
  }
};

int main() {
  // WeatherApp app;
  // app.displayWeather("London");
  cout << "--- OpenWeatherMap ---" << endl;
  WeatherApp oa(make_unique<OpenWeatherMapAPIProvider>());
  oa.displayWeather("London");

  cout << endl << "--- WeatherStack ---" << endl;
  WeatherApp ws(make_unique<WeatherStackProvider>());
  ws.displayWeather("London");
  return 0;
}
