#include <algorithm>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

class WeatherStation; // Forward declaration

class WeatherObserver {
public:
  virtual ~WeatherObserver() {}
  virtual void update(WeatherStation *station) = 0;
};

class WeatherStation {
  double temperature = 0;
  double humidity = 0;
  double pressure = 0;
  vector<WeatherObserver *> observers;

public:
  void registerObserver(WeatherObserver *observer) {
    // TODO: Add observer to the list
    observers.push_back(observer);
  }

  void removeObserver(WeatherObserver *observer) {
    // TODO: Remove observer from the list
    observers.erase(remove(observers.begin(), observers.end(), observer),
                    observers.end());
  }

  void setMeasurements(double temperature, double humidity, double pressure) {
    // TODO: Update fields and notify observers
    this->temperature = temperature;
    this->humidity = humidity;
    this->pressure = pressure;
    notifyObservers();
  }

  double getTemperature() const { return temperature; }
  double getHumidity() const { return humidity; }
  double getPressure() const { return pressure; }

private:
  void notifyObservers() {
    // TODO: Call update on each observer, passing this
    for (auto observer : observers) {
      observer->update(this);
    }
  }
};

class CurrentConditionsDisplay : public WeatherObserver {
public:
  void update(WeatherStation *station) override {
    // TODO: Print "Current Conditions -> Temp: X, Humidity: Y%, Pressure: Z
    // hPa"
    cout << std::fixed << std::setprecision(1);
    cout << "Current Conditions -> Temp: " << station->getTemperature()
         << ", Humidity: " << station->getHumidity()
         << "%, Pressure: " << station->getPressure() << " hPa" << endl;
  }
};

class StatisticsDisplay : public WeatherObserver {
  vector<double> readings;

public:
  void update(WeatherStation *station) override {
    // TODO: Add temperature to readings, compute average, print "Statistics ->
    // Avg Temperature: X"
    readings.push_back(station->getTemperature());
    double sum = accumulate(readings.begin(), readings.end(), 0);
    double average = sum / readings.size();
    cout << "Statistics -> Avg Temperature: " << average << endl;
  }
};

int main() {
  WeatherStation station;
  CurrentConditionsDisplay current;
  StatisticsDisplay stats;
  station.registerObserver(&current);
  station.registerObserver(&stats);
  station.setMeasurements(25.0, 65.0, 1013.0);
  station.setMeasurements(28.0, 70.0, 1012.0);
  station.setMeasurements(22.0, 90.0, 1011.0);
  return 0;
}
