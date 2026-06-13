#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <thread>
#include <vector>

using namespace std;

enum class SignalStatus { RED, GREEN, YELLOW };

class Signal {
private:
  SignalStatus status;
  int greenDuration;
  string id;

public:
  Signal(const string &id, int greenDuration)
      : id(id), greenDuration(greenDuration), status(SignalStatus::RED) {}

  int getGreenDuration() const { return greenDuration; }
  SignalStatus getStatus() const { return status; }

  string getId() { return id; }

  void setSignalStatus() {
    if (status == SignalStatus::RED) {
      status = SignalStatus::GREEN;
    } else if (status == SignalStatus::GREEN) {
      status = SignalStatus::YELLOW;
    } else if (status == SignalStatus::YELLOW) {
      status = SignalStatus::RED;
    }
  }
};

class JunctionController {
private:
  vector<shared_ptr<Signal>> signals;
  int currentSignalIndex;
  int yellowDuration;
  atomic<bool> running{false};
  optional<int> emergencySignalIndex;
  mutable mutex mtx;
  condition_variable cv;
  mutable mutex cvmtx;

public:
  JunctionController(vector<shared_ptr<Signal>> signals, int yellowDuration)
      : signals(signals), yellowDuration(yellowDuration), running(false),
        currentSignalIndex(0) {}

  void start() {
    running = true;
    while (running) {
      auto current = signals[currentSignalIndex];
      current->setSignalStatus();
      cout << "GO for signal: " << currentSignalIndex << endl;

      unique_lock<mutex> cvlock(cvmtx);
      bool emergency = cv.wait_for(
          cvlock, chrono::seconds(current->getGreenDuration()),
          [this]() { return emergencySignalIndex.has_value() || !running; });

      if (emergency) {
        cout << "INTERRUPT" << endl;
        current->setSignalStatus();
        cout << "Yellow for signal: " << currentSignalIndex << endl;
        this_thread::sleep_for(chrono::seconds(yellowDuration));

        current->setSignalStatus();
        cout << "Stop for signal: " << currentSignalIndex << endl;
        this_thread::sleep_for(chrono::seconds(2));
        lock_guard<mutex> lock(mtx);
        currentSignalIndex = emergencySignalIndex.value();
        emergencySignalIndex.reset();

        continue;
      }

      current->setSignalStatus();
      cout << "Yellow for signal: " << currentSignalIndex << endl;
      this_thread::sleep_for(chrono::seconds(yellowDuration));

      current->setSignalStatus();
      cout << "Stop for signal: " << currentSignalIndex << endl;
      this_thread::sleep_for(chrono::seconds(2));

      setNextSignal();
    }
  }

  void stop() {
    running = false;
    cv.notify_one();
  }

  void interrupt(int signalIdx) {
    lock_guard<mutex> lock(mtx);
    emergencySignalIndex = signalIdx;
    cv.notify_one();
  }

private:
  void setNextSignal() {
    currentSignalIndex = (currentSignalIndex + 1) % signals.size();
  }
};

int main() {
  auto s1 = make_shared<Signal>("1", 2);
  auto s2 = make_shared<Signal>("2", 3);
  auto s3 = make_shared<Signal>("3", 1);
  auto s4 = make_shared<Signal>("4", 2);

  auto junctionController = JunctionController({s1, s2, s3, s4}, 5);

  std::thread t1(&JunctionController::start, &junctionController);

  std::thread t2([&junctionController]() {
    this_thread::sleep_for(chrono::seconds(1));
    junctionController.interrupt(2);
  });
  t1.join();
  t2.join();
  return 0;
}
