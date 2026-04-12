#include <iostream>
using namespace std;

class TrafficLight;

class TrafficLightState {
public:
  virtual ~TrafficLightState() = default;
  virtual void change(TrafficLight *context) = 0;
};

class RedState : public TrafficLightState {
public:
  void change(TrafficLight *context) override;
};

class GreenState : public TrafficLightState {
public:
  void change(TrafficLight *context) override;
};

class YellowState : public TrafficLightState {
public:
  void change(TrafficLight *context) override;
};

class TrafficLight {
private:
  TrafficLightState *state;

public:
  TrafficLight();
  ~TrafficLight() { delete state; }
  void setState(TrafficLightState *s) {
    delete state;
    state = s;
  }
  void change() { state->change(this); }
};

TrafficLight::TrafficLight() { state = new RedState(); } // Start with red

void RedState::change(TrafficLight *context) {
  // TODO: Print "RED light - Stop" and transition to GreenState
  cout << "RED light - Stop" << endl;
  context->setState(new GreenState());
}

void GreenState::change(TrafficLight *context) {
  // TODO: Print "GREEN light - Go" and transition to YellowState
  cout << "GREEN light - Go" << endl;
  context->setState(new YellowState());
}

void YellowState::change(TrafficLight *context) {
  // TODO: Print "YELLOW light - Slow down" and transition to RedState
  cout << "YELLOW light - Slow down" << endl;
  context->setState(new RedState());
}

int main() {
  TrafficLight light;
  light.change(); // RED light - Stop
  light.change(); // GREEN light - Go
  light.change(); // YELLOW light - Slow down
  light.change(); // RED light - Stop
  light.change(); // GREEN light - Go
  return 0;
}
