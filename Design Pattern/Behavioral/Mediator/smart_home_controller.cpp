#include <iostream>
#include <string>
#include <vector>
using namespace std;

class SmartDevice;

class SmartHomeMediator {
public:
  virtual ~SmartHomeMediator() = default;
  virtual void notify(SmartDevice *device, const string &event) = 0;
  virtual void registerDevice(SmartDevice *device) = 0;
};

class SmartDevice {
protected:
  SmartHomeMediator *mediator;
  string name;

public:
  SmartDevice(SmartHomeMediator *mediator, const string &name)
      : mediator(mediator), name(name) {}
  virtual ~SmartDevice() = default;
  string getName() const { return name; }
  virtual void receiveCommand(const string &command) = 0;
};

// Colleagues
class Thermostat : public SmartDevice {
  int temperature = 72;

public:
  Thermostat(SmartHomeMediator *mediator, const string &name)
      : SmartDevice(mediator, name) {}

  void setTemperature(int temp) {
    // TODO: Set temperature field to temp
    // TODO: Print "Name: Temperature set to X"
    // TODO: If temp > 75, notify mediator with "TEMP_HIGH"
    // TODO: If temp <= 75, notify mediator with "TEMP_NORMAL"
    cout << "Living Room Thermostat: Temperature set to " << temp << endl;
    if (temp > 75) {
      mediator->notify(this, "TEMP_HIGH");
    } else {
      mediator->notify(this, "TEMP_NORMAL");
    }
  }

  void receiveCommand(const string &command) override {
    // TODO: Handle commands if needed
  }
};

class Lights : public SmartDevice {
public:
  Lights(SmartHomeMediator *mediator, const string &name)
      : SmartDevice(mediator, name) {}

  void receiveCommand(const string &command) override {
    // TODO: Print "Name: Turned ON" or "Name: Turned OFF" based on command
    cout << "Living Room Lights: Turned " << command << endl;
  }
};

class AirConditioner : public SmartDevice {
public:
  AirConditioner(SmartHomeMediator *mediator, const string &name)
      : SmartDevice(mediator, name) {}

  void receiveCommand(const string &command) override {
    // TODO: Print "Name: Turned ON" or "Name: Turned OFF" based on command
    cout << "Central AC: Turned " << command << endl;
  }
};

// Concrete Mediator
class SmartHomeHub : public SmartHomeMediator {
  vector<SmartDevice *> devices;

public:
  void registerDevice(SmartDevice *device) override {
    // TODO: Add device to the list
    devices.push_back(device);
  }

  void notify(SmartDevice *device, const string &event) override {
    // TODO: Print "[SmartHomeHub] EVENT detected. Action message."
    // TODO: "TEMP_HIGH" -> find AirConditioner, send "ON" command
    // TODO: "TEMP_NORMAL" -> find AirConditioner, send "OFF" command
    // TODO: "NIGHT_MODE" -> find Lights, send "OFF" command
    // cout<<"[SmartHomeHub] "<<event<<" detected.  "<<"some command
    // here"<<"."<<endl;
    if (event == "TEMP_HIGH") {
      cout << "[SmartHomeHub] " << event << " detected. Turning on AC." << endl;
      for (SmartDevice *device : devices) {
        if (dynamic_cast<AirConditioner *>(device)) {
          device->receiveCommand("ON");
        }
      }
    } else if (event == "TEMP_NORMAL") {
      cout << "[SmartHomeHub] " << event << " detected. Turning off AC."
           << endl;
      for (SmartDevice *device : devices) {
        if (dynamic_cast<AirConditioner *>(device)) {
          device->receiveCommand("OFF");
        }
      }
    } else if (event == "NIGHT_MODE") {
      cout << "[SmartHomeHub] " << event << " activated. Turning off lights."
           << endl;
      for (SmartDevice *device : devices) {
        if (dynamic_cast<Lights *>(device)) {
          device->receiveCommand("OFF");
        }
      }
    }
  }
};

int main() {
  SmartHomeHub hub;
  Thermostat thermostat(&hub, "Living Room Thermostat");
  Lights lights(&hub, "Living Room Lights");
  AirConditioner ac(&hub, "Central AC");
  hub.registerDevice(&thermostat);
  hub.registerDevice(&lights);
  hub.registerDevice(&ac);
  thermostat.setTemperature(80);
  thermostat.setTemperature(70);
  hub.notify(&lights, "NIGHT_MODE");
  return 0;
}
