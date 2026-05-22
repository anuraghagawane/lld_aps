#include <chrono>
#include <climits>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <set>
#include <thread>
#include <vector>

using namespace std;

enum class Direction { UP, DOWN, IDLE };

enum class ElevatorState { IDLE, MOVING, DOOR_OPEN };

string directionToString(Direction dir) {
  switch (dir) {
  case Direction::UP:
    return "UP";
  case Direction::DOWN:
    return "DOWN";
  case Direction::IDLE:
    return "IDLE";
  }

  return "";
}

class Elevator {
private:
  int id;
  int currentFloor;
  int maxCapacity;
  int currentLoad;

  Direction direction;
  ElevatorState state;

  set<int> upRequests;
  set<int, greater<int>> downRequests;

  mutable mutex mtx;
  condition_variable cv;

  bool running;
  thread worker;

  void processRequests() {
    while (running) {
      unique_lock<mutex> lock(mtx);

      cv.wait(lock, [this]() {
        return !upRequests.empty() || !downRequests.empty() || !running;
      });

      if (!running)
        break;

      state = ElevatorState::MOVING;

      if (direction == Direction::IDLE) {
        if (!upRequests.empty()) {
          direction = Direction::UP;
        } else if (!downRequests.empty()) {
          direction = Direction::DOWN;
        }
      }

      if (direction == Direction::UP) {
        processUpRequests(lock);
      } else if (direction == Direction::DOWN) {
        processDownRequests(lock);
      }

      if (upRequests.empty() && downRequests.empty()) {
        direction = Direction::IDLE;
        state = ElevatorState::IDLE;
      }
    }
  }

  void processUpRequests(unique_lock<mutex> &lock) {
    while (!upRequests.empty()) {
      int targetFloor = *upRequests.begin();
      upRequests.erase(upRequests.begin());
      lock.unlock();
      moveToFloor(targetFloor);
      lock.lock();
    }

    if (!downRequests.empty()) {
      direction = Direction::DOWN;
    }
  }

  void processDownRequests(unique_lock<mutex> &lock) {
    while (!downRequests.empty()) {
      int targetFloor = *downRequests.begin();
      downRequests.erase(downRequests.begin());

      lock.unlock();

      moveToFloor(targetFloor);

      lock.lock();
    }

    if (!upRequests.empty()) {
      direction = Direction::UP;
    }
  }

  void moveToFloor(int targetFloor) {
    cout << "[Elevator " << id << "] Moving from Floor " << currentFloor
         << " to Floor " << targetFloor << endl;

    while (currentFloor != targetFloor) {
      this_thread::sleep_for(chrono::milliseconds(500));

      if (currentFloor < targetFloor) {
        currentFloor++;
      } else {
        currentFloor--;
      }

      cout << "[Elevator " << id << "] Current Floor: " << currentFloor << endl;
    }

    openDoor();
    closeDoor();
  }

  void openDoor() {
    state = ElevatorState::DOOR_OPEN;

    cout << "[Elevator " << id << "] Door Opening at Floor " << currentFloor
         << endl;

    this_thread::sleep_for(chrono::milliseconds(500));
  }

  void closeDoor() {
    cout << "[Elevator " << id << "] Door Closing" << endl;

    this_thread::sleep_for(chrono::milliseconds(500));

    state = ElevatorState::MOVING;
  }

public:
  Elevator(int id, int maxCapacity)
      : id(id), currentFloor(0), maxCapacity(maxCapacity), currentLoad(0),
        direction(Direction::IDLE), state(ElevatorState::IDLE), running(true) {
    worker = thread(&Elevator::processRequests, this);
  }

  void addRequest(int floor) {
    lock_guard<mutex> lock(mtx);

    if (floor == currentFloor) {
      return;
    }

    if (floor > currentFloor) {
      upRequests.insert(floor);
    } else {
      downRequests.insert(floor);
    }

    cout << "[Elevator " << id << "] Request Added for Floor " << floor << endl;

    cv.notify_one();
  }

  int getCurrentFloor() const {
    lock_guard<mutex> lock(mtx);
    return currentFloor;
  }

  Direction getDirection() const {
    lock_guard<mutex> lock(mtx);
    return direction;
  }

  int getId() const { return id; }

  ~Elevator() {
    {
      lock_guard<mutex> lock(mtx);
      running = false;
    }

    cv.notify_one();

    if (worker.joinable()) {
      worker.join();
    }
  }
};

class ElevatorController {
private:
  vector<shared_ptr<Elevator>> elevators;
  mutable mutex mtx;

private:
  shared_ptr<Elevator> selectBestElevator(int requestedFloor,
                                          Direction requestedDirection) {
    shared_ptr<Elevator> bestElevator = nullptr;
    int minDistance = INT_MAX;

    for (auto &elevator : elevators) {
      int currentFloor = elevator->getCurrentFloor();
      Direction elevatorDirection = elevator->getDirection();

      bool compatible = false;

      if (elevatorDirection == Direction::IDLE) {
        compatible = true;
      }

      else if (elevatorDirection == requestedDirection) {
        if (requestedDirection == Direction::UP &&
            currentFloor <= requestedFloor) {
          compatible = true;
        }

        if (requestedDirection == Direction::DOWN &&
            currentFloor >= requestedFloor) {
          compatible = true;
        }
      }

      if (compatible) {
        int distance = abs(currentFloor - requestedFloor);
        if (distance < minDistance) {
          minDistance = distance;
          bestElevator = elevator;
        }
      }
    }

    if (!bestElevator && !elevators.empty()) {
      bestElevator = elevators[0];
    }

    return bestElevator;
  }

public:
  void addElevator(shared_ptr<Elevator> elevator) {
    lock_guard<mutex> lock(mtx);
    elevators.push_back(elevator);
  }

  void requestElevator(int floor, Direction direction) {
    lock_guard<mutex> lock(mtx);

    auto elevator = selectBestElevator(floor, direction);

    if (!elevator) {
      cout << "No elevator available\n";
      return;
    }

    cout << "[Controller] Assigning Elevator " << elevator->getId()
         << " to Floor " << floor << " Direction "
         << directionToString(direction) << endl;

    elevator->addRequest(floor);
  }
};

int main() {
  auto controller = make_shared<ElevatorController>();

  auto elevator1 = make_shared<Elevator>(1, 5);
  auto elevator2 = make_shared<Elevator>(2, 5);

  controller->addElevator(elevator1);
  controller->addElevator(elevator2);

  controller->requestElevator(5, Direction::UP);

  this_thread::sleep_for(chrono::seconds(1));

  controller->requestElevator(2, Direction::UP);

  this_thread::sleep_for(chrono::seconds(1));

  controller->requestElevator(8, Direction::DOWN);

  this_thread::sleep_for(chrono::seconds(15));
  return 0;
}
