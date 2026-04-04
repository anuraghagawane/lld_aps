#include <iostream>
#include <string>
using namespace std;

// Subsystem: Controls smart lights in the house
class SmartLightsSystem {
public:
  void on() {
    // TODO: Print "Lights: Turned on."
    cout << "Lights: Turned on." << endl;
  }

  void off() {
    // TODO: Print "Lights: Turned off."
    cout << "Lights: Turned off." << endl;
  }
};

// Subsystem: Controls the thermostat temperature and mode
class Thermostat {
  string mode;

public:
  void setTemperature(int degrees) {
    // TODO: Print "Thermostat: Mode set to {mode}. Temperature set to
    // {degrees}C."
    cout << "Thermostat: Mode set to " << mode << ". Temperature set to "
         << degrees << "C." << endl;
  }

  void setMode(const string &mode) {
    // TODO: Store the mode
    this->mode = mode;
  }
};

// Subsystem: Controls the home security system
class SecuritySystem {
public:
  void arm() {
    // TODO: Print "Security: System armed."
    cout << "Security: System armed." << endl;
  }

  void disarm() {
    // TODO: Print "Security: System disarmed."
    cout << "Security: System disarmed." << endl;
  }
};

// Facade: Provides simplified methods to control all smart home subsystems
class SmartHomeFacade {
  SmartLightsSystem &lights;
  Thermostat &thermostat;
  SecuritySystem &security;

public:
  SmartHomeFacade(SmartLightsSystem &lights, Thermostat &thermostat,
                  SecuritySystem &security)
      : lights(lights), thermostat(thermostat), security(security) {
    // TODO: References are already initialized via initializer list
  }

  void leaveHome() {
    // TODO: Print "--- Leaving Home ---"
    // TODO: Turn off lights, set thermostat to eco mode (18C), arm security
    // TODO: Print "--- Home secured ---"
    cout << "--- Leaving Home ---" << endl;
    lights.off();
    thermostat.setMode("eco");
    thermostat.setTemperature(18);
    security.arm();
    cout << "--- Home secured ---" << endl;
  }

  void arriveHome() {
    // TODO: Print "--- Arriving Home ---"
    // TODO: Turn on lights, set thermostat to comfort mode (22C), disarm
    // security
    // TODO: Print "--- Welcome home! ---"
    cout << "--- Arriving Home ---" << endl;
    lights.on();
    thermostat.setMode("comfort");
    thermostat.setTemperature(22);
    security.disarm();
    cout << "--- Welcome home! ---" << endl;
  }
};

int main() {
  SmartLightsSystem lights;
  Thermostat thermostat;
  SecuritySystem security;

  SmartHomeFacade home(lights, thermostat, security);
  home.leaveHome();
  cout << endl;
  home.arriveHome();
  return 0;
}
