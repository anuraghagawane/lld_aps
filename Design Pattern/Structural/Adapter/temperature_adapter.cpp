#include <cstdio>
using namespace std;

class Thermometer {
public:
  virtual double getTemperature() = 0; // Returns Celsius
  virtual ~Thermometer() {}
};

class CelsiusSensor : public Thermometer {
public:
  double getTemperature() override { return 25.0; }
};

class FahrenheitSensor {
public:
  double readFahrenheit() { return 98.6; }
};

class FahrenheitSensorAdapter : public Thermometer {
  // TODO: Declare a FahrenheitSensor* field
  FahrenheitSensor *fs;

public:
  FahrenheitSensorAdapter(FahrenheitSensor *sensor) { fs = sensor; }

  double getTemperature() override {
    // TODO: Read the Fahrenheit value from the sensor
    // TODO: Convert Fahrenheit to Celsius using (F - 32) * 5.0 / 9.0
    return (fs->readFahrenheit() - 32) * 5.0 /
           9.0; // TODO: Return the converted Celsius value
  }
};

int main() {
  CelsiusSensor celsius;
  printf("Celsius sensor: %.1f C\n", celsius.getTemperature());

  // TODO: Uncomment the lines below after implementing the adapter
  FahrenheitSensor sensor;
  FahrenheitSensorAdapter adapted(&sensor);
  printf("Fahrenheit sensor (adapted): %.1f C\n", adapted.getTemperature());
  return 0;
}
