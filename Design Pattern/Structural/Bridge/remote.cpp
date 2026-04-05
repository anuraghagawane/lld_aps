#include <algorithm>
#include <iostream>
using namespace std;

class Device {
public:
  virtual ~Device() {}
  virtual bool isEnabled() = 0;
  virtual void enable() = 0;
  virtual void disable() = 0;
  virtual int getVolume() = 0;
  virtual void setVolume(int volume) = 0;
};

class TV : public Device {
private:
  bool enabled = false;
  int volume = 30;

public:
  bool isEnabled() override { return enabled; }

  void enable() override {
    enabled = true;
    cout << "TV: Turned ON" << endl;
  }

  void disable() override {
    enabled = false;
    cout << "TV: Turned OFF" << endl;
  }

  int getVolume() override { return volume; }

  void setVolume(int vol) override {
    volume = max(0, min(100, vol));
    cout << "TV: Volume set to " << volume << endl;
  }
};

// ConcreteImplementor: Radio
class Radio : public Device {
private:
  bool enabled = false;
  int volume = 20;

public:
  bool isEnabled() override { return enabled; }

  void enable() override {
    enabled = true;
    cout << "Radio: Turned ON" << endl;
  }

  void disable() override {
    enabled = false;
    cout << "Radio: Turned OFF" << endl;
  }

  int getVolume() override { return volume; }

  void setVolume(int vol) override {
    volume = max(0, min(100, vol));
    cout << "Radio: Volume set to " << volume << endl;
  }
};

// Abstraction
class Remote {
protected:
  Device *device;

public:
  Remote(Device *device) : device(device) {}
  virtual ~Remote() {}

  void togglePower() {
    if (device->isEnabled()) {
      device->disable();
    } else {
      device->enable();
    }
  }

  void volumeUp() { device->setVolume(device->getVolume() + 10); }

  void volumeDown() { device->setVolume(device->getVolume() - 10); }
};

class BasicRemote : public Remote {
public:
  BasicRemote(Device *device) : Remote(device) {}
};

class AdvancedRemote : public Remote {
public:
  AdvancedRemote(Device *device) : Remote(device) {}

  void mute() {
    device->setVolume(0);
    cout << "AdvancedRemote: Muted" << endl;
  }
};

int main() {
  cout << "--- Basic Remote with TV ---" << endl;
  TV tv;
  BasicRemote basicRemote(&tv);
  basicRemote.togglePower();
  basicRemote.volumeUp();
  basicRemote.volumeUp();
  basicRemote.volumeDown();

  cout << "\n--- Advanced Remote with Radio ---" << endl;
  Radio radio;
  AdvancedRemote advancedRemote(&radio);
  advancedRemote.togglePower();
  advancedRemote.volumeUp();
  advancedRemote.mute();

  cout << "\n--- Advanced Remote with TV ---" << endl;
  AdvancedRemote tvAdvanced(&tv);
  tvAdvanced.volumeUp();
  tvAdvanced.mute();

  return 0;
}
