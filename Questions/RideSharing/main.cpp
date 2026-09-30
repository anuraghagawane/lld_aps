#include <atomic>
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <unordered_map>
#include <vector>

using namespace std;

class User {
private:
  string id;
  string name;

public:
  User(const string &id, const string &name) : id(id), name(name) {}

  string getId() const { return id; }
  string getName() const { return name; }
};

struct Location {
  double lat;
  double lng;
};

enum class RideType { STANDARD, PREMIUM };

class Driver : public User {
private:
  bool availability;
  Location currLocation;
  RideType rideTypeEligibility;
  mutable mutex mtx;

public:
  Driver(const string &id, const string &name, RideType rideTypeEligibility)
      : User(id, name), rideTypeEligibility(rideTypeEligibility),
        availability(true) {}

  bool isAvailable() const { return availability; }
  Location getCurrLocation() const { return currLocation; }
  RideType getRideTypeEligibility() const { return rideTypeEligibility; }

  void setAvailable() { availability = true; }
  void setUnavailable() { availability = false; }

  void updateLocation(Location newLocation) { currLocation = newLocation; }

  mutex &getMutex() { return mtx; }
};

enum class RideStatus {
  REQUESTED,
  DRIVER_ACCEPTED,
  IN_TRANSIT,
  REACHED_DESTINATION,
  COMPLETED,
  FAILED,
};

double calculateDistanceBetweenPoints(Location l1, Location l2) {
  return sqrt(pow(l1.lat - l2.lat, 2) + pow(l1.lng - l2.lng, 2));
}

class Ride {
private:
  string id;
  string passengerId;
  string driverId;
  Location pickup;
  Location destination;
  Location currLocation;
  RideType rideType;
  RideStatus status;
  double accumulatedDistance;
  string paymentId;
  chrono::system_clock::time_point startTime;
  chrono::system_clock::time_point endTime;
  mutex mtx;

public:
  Ride(const string &id, const string &passengerId, Location pickup,
       Location destination, RideType rideType)
      : id(id), passengerId(passengerId), pickup(pickup),
        destination(destination), rideType(rideType), accumulatedDistance(0),
        status(RideStatus::REQUESTED) {
    currLocation = pickup;
  }

  string getId() const { return id; }
  mutex &getMutex() { return mtx; }
  string getDriverId() const { return driverId; }
  void setDriverId(const string &driverId) { this->driverId = driverId; }

  void updateDistanceAndLocation(Location newLocation) {
    lock_guard<mutex> lock(mtx);
    accumulatedDistance +=
        calculateDistanceBetweenPoints(currLocation, newLocation);
    currLocation = newLocation;
  }

  double getTotalDistance() {
    lock_guard<mutex> lock(mtx);
    return accumulatedDistance;
  }

  double getTotalTime() {
    chrono::duration<double> elapsed_seconds = endTime - startTime;
    return elapsed_seconds.count();
  }

  string getPaymentId() const { return paymentId; }
  void setPaymentId(const string &paymentId) { this->paymentId = paymentId; }

  RideType getRideType() const { return rideType; }

  RideStatus getStatus() const { return status; }
  // set statuses
  void markDriverAccepted() {
    if (status != RideStatus::REQUESTED) {
      throw runtime_error("Cannot mark as accpeted");
    }
    status = RideStatus::DRIVER_ACCEPTED;
  }

  void markInTransit() {
    if (status != RideStatus::DRIVER_ACCEPTED) {
      throw runtime_error("Cannot mark as in transit");
    }
    startTime = chrono::system_clock::now();
    status = RideStatus::IN_TRANSIT;
  }

  void markReachedDestination() {
    if (status != RideStatus::IN_TRANSIT) {
      throw runtime_error("Cannot mark as REACHED_DESTINATION");
    }
    status = RideStatus::REACHED_DESTINATION;
    endTime = chrono::system_clock::now();
  }

  void markCompleted() {
    if (status != RideStatus::REACHED_DESTINATION) {
      throw runtime_error("Cannot mark as COMPLETED");
    }
    status = RideStatus::COMPLETED;
  }

  void markFailed() {
    if (status != RideStatus::REQUESTED) {
      throw runtime_error("Cannot mark as failed/cancelled");
    }
    status = RideStatus::FAILED;
  }
};

enum class PaymentStatus { PENDING, DONE, FAILED };

class Payment {
private:
  string id;
  int amount;
  PaymentStatus status;

public:
  Payment(const string &id) : id(id), status(PaymentStatus::PENDING) {}

  string getId() const { return id; }
  int getAmount() const { return amount; }
  PaymentStatus getStatus() const { return status; }

  bool isDone() { return status == PaymentStatus::DONE; }

  void setAmount(int amount) { this->amount = amount; }

  void markPaid() { status = PaymentStatus::DONE; }
  void markFailed() { status = PaymentStatus::FAILED; }
};

struct FareConfig {
  double baseFare;
  double perKmFare;
  double perMinPrice;
};

class FareService {
private:
  unordered_map<RideType, FareConfig> fareConfig;

public:
  FareService() {
    fareConfig[RideType::STANDARD] = FareConfig{100, 10, 5};
    fareConfig[RideType::PREMIUM] = FareConfig{200, 20, 7};
  }

  int calculateFair(RideType rideType, double distance, double time) {
    auto cfg = fareConfig[rideType];
    return ceil(cfg.baseFare + distance * cfg.perKmFare +
                time * cfg.perMinPrice);
  }
};

class PaymentService {
private:
  unordered_map<string, unique_ptr<Payment>> payments;
  mutex mtx;
  atomic<int> counter;

public:
  string initiatePayment() {
    lock_guard<mutex> lock(mtx);
    const string id = "PAY_" + to_string(counter.fetch_add(1));
    payments[id] = make_unique<Payment>(id);
    return id;
  }

  void setAmount(const string &paymentId, int amount) {
    lock_guard<mutex> lock(mtx);
    if (!payments.count(paymentId)) {
      throw runtime_error("Payment not found!");
    }

    payments[paymentId]->setAmount(amount);
  }

  bool isDone(const string &paymentId) {
    lock_guard<mutex> lock(mtx);
    if (!payments.count(paymentId)) {
      throw runtime_error("Payment not found!");
    }

    return payments[paymentId]->isDone();
  }

  void pay(const string &paymentId) { markCompleted(paymentId); }

  void markCompleted(const string &paymentId) {
    lock_guard<mutex> lock(mtx);
    if (!payments.count(paymentId)) {
      throw runtime_error("Payment not found!");
    }

    payments[paymentId]->markPaid();
  }
};

class DriverService {
private:
  unordered_map<string, shared_ptr<Driver>> drivers;
  atomic<int> counter = 0;

public:
  string addDriver(const string &name, RideType rideTypeEligibility) {
    const string id = "DRV_" + to_string(counter.fetch_add(1));

    drivers[id] = make_shared<Driver>(id, name, rideTypeEligibility);

    return id;
  }

  void acceptRide(shared_ptr<Driver> driver, unique_ptr<Ride> &ride) {
    lock_guard<mutex> driverLock(driver->getMutex());

    if (!driver->isAvailable()) {
      throw runtime_error("Cannot accept the ride");
    }

    if (ride->getRideType() != driver->getRideTypeEligibility()) {
      throw runtime_error("ride type mismatch");
    }

    lock_guard<mutex> rideLock(ride->getMutex());
    if (ride->getStatus() != RideStatus::REQUESTED) {
      throw runtime_error("Ride already accepted");
    }

    ride->setDriverId(driver->getId());
    ride->markDriverAccepted();
    driver->setUnavailable();

    cout << "Ride: " << ride->getId() << " accpeted by driver "
         << driver->getId() << endl;
  }

  void updateLocation(const string &driverId, Location newLocation) {
    if (!drivers.count(driverId)) {
      throw runtime_error("driver not present");
    }

    auto &driver = drivers[driverId];

    driver->updateLocation(newLocation);
  }

  void completeRide(const string &driverId) {
    drivers[driverId]->setAvailable();
  }

  void findAndNotifyDriver(const string &rideId, Location pickup,
                           RideType rideType) {
    vector<shared_ptr<Driver>> nearByDrivers;
    for (auto &[driverId, driver] : drivers) {
      double currentDistance =
          calculateDistanceBetweenPoints(driver->getCurrLocation(), pickup);
      if (driver->isAvailable() && currentDistance < 500 &&
          driver->getRideTypeEligibility() == rideType) {
        nearByDrivers.push_back(driver);
      }
    }

    for (auto &driver : nearByDrivers) {
      cout << "Hi, " << driver->getName() << "\n New ride request: " << rideId
           << endl;
    }
  }

  shared_ptr<Driver> getDriver(const string &driverId) {
    return drivers[driverId];
  }
};

class RideService {
private:
  unordered_map<string, unique_ptr<Ride>> rides;
  DriverService &driverService;
  PaymentService &paymentService;
  FareService fareService;

  atomic<int> counter = 0;

public:
  RideService(DriverService &driverService, PaymentService &paymentService)
      : driverService(driverService), paymentService(paymentService) {}

  pair<string, string> requestRide(const string &passengerId, Location pickup,
                                   Location destination, RideType rideType) {
    const string id = "RIDE_" + to_string(counter.fetch_add(1));

    rides[id] =
        make_unique<Ride>(id, passengerId, pickup, destination, rideType);

    auto &ride = rides[id];

    string paymentId = paymentService.initiatePayment();

    ride->setPaymentId(paymentId);

    driverService.findAndNotifyDriver(id, pickup, rideType);

    return {id, paymentId};
  }

  void updateCurrentLocation(const string &rideId, Location newLocation) {
    if (!rides.count(rideId)) {
      throw runtime_error("ride not found");
    }

    auto &ride = rides[rideId];

    ride->updateDistanceAndLocation(newLocation);
    driverService.updateLocation(ride->getDriverId(), newLocation);

    cout << ride->getId() << "'s new location is " << newLocation.lat << " "
         << newLocation.lng << endl;
  }

  void acceptRide(const string &rideId, const string &driverId) {
    if (!rides.count(rideId)) {
      throw runtime_error("ride not found");
    }

    auto &ride = rides[rideId];

    auto driver = driverService.getDriver(driverId);

    driverService.acceptRide(driver, ride);
  }

  void markInTransit(const string &rideId) {
    if (!rides.count(rideId)) {
      throw runtime_error("ride not found");
    }

    rides[rideId]->markInTransit();
    cout << "in transit" << endl;
  }

  void markReachedDestination(const string &rideId) {
    if (!rides.count(rideId)) {
      throw runtime_error("ride not found");
    }
    auto &ride = rides[rideId];
    ride->markReachedDestination();
    int fair = fareService.calculateFair(
        ride->getRideType(), ride->getTotalDistance(), ride->getTotalTime());

    paymentService.setAmount(ride->getPaymentId(), fair);
    cout << "reached destination, fair: rs." << fair << endl;
  }

  void markCompleted(const string &rideId, const string &driverId) {
    if (!rides.count(rideId)) {
      throw runtime_error("ride not found");
    }
    auto &ride = rides[rideId];

    if (ride->getDriverId() != driverId) {
      throw runtime_error("You are not authorized to mark this as complelted");
    }

    ride->markCompleted();
    driverService.completeRide(ride->getDriverId());
    cout << "Ride completed" << endl;
  }

  void cancelRide(const string &rideId) {
    if (!rides.count(rideId)) {
      throw runtime_error("ride not found");
    }
    auto &ride = rides[rideId];

    string driverId = ride->getDriverId();
    shared_ptr<Driver> driver;
    if (driverId != "") {
      driver = driverService.getDriver(driverId);
      driver->getMutex().lock();
      driver->setAvailable();
    }

    lock_guard<mutex> lock(ride->getMutex());

    ride->markFailed();

    if (driver) {
      driver->getMutex().unlock();
    }
  }
};

int main() {
  PaymentService paymentService;
  DriverService driverService;
  RideService rideService(driverService, paymentService);

  auto d1 = driverService.addDriver("anurag", RideType::STANDARD);

  auto user = User("U1", "ah");
  auto [rideId, payementId] = rideService.requestRide(
      user.getId(), Location{1.1, 1.1}, Location{5, 5}, RideType::STANDARD);

  // rideService.cancelRide(rideId);
  // return 0;
  rideService.acceptRide(rideId, d1);
  rideService.markInTransit(rideId);
  rideService.updateCurrentLocation(rideId, Location{2.1, 2.2});
  rideService.updateCurrentLocation(rideId, Location{4.1, 4.2});
  rideService.updateCurrentLocation(rideId, Location{5, 5});
  rideService.markReachedDestination(rideId);
  paymentService.pay(payementId);
  rideService.markCompleted(rideId, d1);

  return 0;
}
