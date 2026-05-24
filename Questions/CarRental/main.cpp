#include <iostream>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

using namespace std;

enum class CarStatus { AVAILABLE, RESERVED, UNDER_MAINTENANCE, OUT_OF_SERVICE };

enum class ReservationStatus { CREATED, CONFIRMED, CANCELLED, COMPLETED };

enum class PaymentStatus { PENDING, SUCCESS, FAILED, REFUNDED };

enum class CarType { SUV, SEDAN, HATCHBACK };

class Car {
private:
  int id;
  string make;
  string model;
  int year;
  string licensePlate;
  double pricePerDay;
  CarType type;
  CarStatus status;

public:
  Car(int id, string make, string model, int year, string licensePlate,
      double pricePerDay, CarType type)
      : id(id), make(make), model(model), year(year),
        licensePlate(licensePlate), pricePerDay(pricePerDay), type(type),
        status(CarStatus::AVAILABLE) {}

  int getId() const { return id; }

  double getPricePerDay() const { return pricePerDay; }

  CarType getType() const { return type; }

  CarStatus getStatus() const { return status; }

  void setStatus(CarStatus newStatus) { status = newStatus; }

  void display() const {
    cout << "Car ID: " << id << " | " << make << " " << model
         << " | Price Per Day: " << pricePerDay << endl;
  }
};

class Customer {
private:
  int id;
  string name;
  string contact;
  string licenseNumber;

public:
  Customer(int id, string name, string contact, string licenseNumber)
      : id(id), name(name), contact(contact), licenseNumber(licenseNumber) {}

  int getId() const { return id; }

  string getName() const { return name; }
};

class Reservation {
private:
  int reservationId;
  shared_ptr<Car> car;
  shared_ptr<Customer> customer;
  int startDate;
  int endDate;
  double totalPrice;
  ReservationStatus status;

public:
  Reservation(int reservationId, shared_ptr<Car> car,
              shared_ptr<Customer> customer, int startDate, int endDate)
      : reservationId(reservationId), car(car), customer(customer),
        startDate(startDate), endDate(endDate),
        status(ReservationStatus::CREATED) {
    totalPrice = calculatePrice();
  }

  int getReservationId() const { return reservationId; }

  shared_ptr<Car> getCar() const { return car; }

  int getStartDate() const { return startDate; }

  int getEndDate() const { return endDate; }

  ReservationStatus getStatus() const { return status; }

  void confirmReservation() { status = ReservationStatus::CONFIRMED; }

  void cancelReservation() { status = ReservationStatus::CANCELLED; }

  double calculatePrice() {
    int totalDays = endDate - startDate + 1;

    return totalDays * car->getPricePerDay();
  }

  void display() const {
    cout << "Reservation ID: " << reservationId
         << " | Customer: " << customer->getName()
         << " | Total Price: " << totalPrice << endl;
  }
};

class Payment {
private:
  int paymentId;
  double amount;
  PaymentStatus status;

public:
  Payment(int paymentId, double amount)
      : paymentId(paymentId), amount(amount), status(PaymentStatus::PENDING) {}

  bool processPayment() {
    status = PaymentStatus::SUCCESS;
    return true;
  }

  void refund() { status = PaymentStatus::REFUNDED; }
};

class Filter {
public:
  virtual vector<shared_ptr<Car>>
  apply(const vector<shared_ptr<Car>> &cars) = 0;

  virtual ~Filter() {}
};

class TypeFilter : public Filter {
private:
  CarType type;

public:
  TypeFilter(CarType type) : type(type) {}

  vector<shared_ptr<Car>> apply(const vector<shared_ptr<Car>> &cars) override {
    vector<shared_ptr<Car>> result;

    for (auto &car : cars) {
      if (car->getType() == type) {
        result.push_back(car);
      }
    }

    return result;
  }
};

class PriceFilter : public Filter {
private:
  double maxPrice;

public:
  PriceFilter(double maxPrice) : maxPrice(maxPrice) {}

  vector<shared_ptr<Car>> apply(const vector<shared_ptr<Car>> &cars) override {
    vector<shared_ptr<Car>> result;

    for (auto &car : cars) {
      if (car->getPricePerDay() <= maxPrice) {
        result.push_back(car);
      }
    }

    return result;
  }
};

class AvailabilityFilter : public Filter {
public:
  vector<shared_ptr<Car>> apply(const vector<shared_ptr<Car>> &cars) override {
    vector<shared_ptr<Car>> result;

    for (auto &car : cars) {
      if (car->getStatus() == CarStatus::AVAILABLE) {
        result.push_back(car);
      }
    }

    return result;
  }
};

class SearchService {
private:
  vector<shared_ptr<Filter>> filters;

public:
  void addFilter(shared_ptr<Filter> filter) { filters.push_back(filter); }

  vector<shared_ptr<Car>> search(vector<shared_ptr<Car>> cars) {
    for (auto &filter : filters) {
      cars = filter->apply(cars);
    }

    return cars;
  }

  void clearFilters() { filters.clear(); }
};

class CarInventory {
private:
  vector<shared_ptr<Car>> cars;

public:
  void addCar(shared_ptr<Car> car) { cars.push_back(car); }

  vector<shared_ptr<Car>> getAllCars() { return cars; }

  shared_ptr<Car> getCarById(int carId) {
    for (auto &car : cars) {
      if (car->getId() == carId) {
        return car;
      }
    }
    return nullptr;
  }
};

class ReservationService {
private:
  vector<shared_ptr<Reservation>> reservations;
  unordered_map<int, mutex> carLocks;

public:
  bool isOverLapping(int existingStart, int existingEnd, int newStart,
                     int newEnd) {
    return !(existingEnd < newStart || existingStart > newEnd);
  }

  bool checkAvailability(shared_ptr<Car> car, int startDate, int endDate) {
    for (auto &reservation : reservations) {
      if (reservation->getCar()->getId() == car->getId() &&
          reservation->getStatus() != ReservationStatus::CANCELLED) {
        if (isOverLapping(reservation->getStartDate(),
                          reservation->getEndDate(), startDate, endDate)) {
          return false;
        }
      }
    }

    return true;
  }

  shared_ptr<Reservation> createReservation(int reservationId,
                                            shared_ptr<Customer> customer,
                                            shared_ptr<Car> car, int startDate,
                                            int endDate) {
    lock_guard<mutex> guard(carLocks[car->getId()]);

    if (!checkAvailability(car, startDate, endDate)) {
      cout << "Car not available for selected dates" << endl;
      return nullptr;
    }

    auto reservation = make_shared<Reservation>(reservationId, car, customer,
                                                startDate, endDate);

    reservation->confirmReservation();

    reservations.push_back(reservation);

    cout << "Reservation created successfully" << endl;

    return reservation;
  }

  void cancelReservation(int reservationId) {
    for (auto &reservation : reservations) {
      for (auto &reservation : reservations) {
        if (reservation->getReservationId() == reservationId) {
          reservation->cancelReservation();
          cout << "Reservation cancelled" << endl;
          return;
        }
      }
    }
  }
};

class PaymentService {
public:
  bool makePayment(shared_ptr<Payment> payment) {
    return payment->processPayment();
  }

  void refund(shared_ptr<Payment> payment) { payment->refund(); }
};

class RentalSystem {
private:
  CarInventory inventory;
  SearchService searchService;
  ReservationService reservationService;
  PaymentService paymentService;

public:
  CarInventory &getInventory() { return inventory; }

  SearchService &getSearchService() { return searchService; }

  ReservationService &getReservationService() { return reservationService; }

  PaymentService &getPaymentService() { return paymentService; }
};

int main() {
  RentalSystem rentalSystem;

  auto car1 = make_shared<Car>(1, "Toyota", "Fortuner", 2022, "MH12AB1234",
                               5000, CarType::SUV);
  auto car2 = make_shared<Car>(2, "Honda", "City", 2021, "MH14XY5678", 3000,
                               CarType::SEDAN);

  rentalSystem.getInventory().addCar(car1);
  rentalSystem.getInventory().addCar(car2);

  auto &searchService = rentalSystem.getSearchService();

  searchService.addFilter(make_shared<TypeFilter>(CarType::SUV));
  searchService.addFilter(make_shared<PriceFilter>(6000));
  searchService.addFilter(make_shared<AvailabilityFilter>());

  auto cars = searchService.search(rentalSystem.getInventory().getAllCars());

  cout << "Available Cars: " << endl;

  for (auto &car : cars) {
    car->display();
  }

  auto customer = make_shared<Customer>(1, "Anurag", "9999999999", "DL12345");

  auto reservation = rentalSystem.getReservationService().createReservation(
      1, customer, car1, 1, 5);

  if (reservation != nullptr) {
    reservation->display();
  }

  auto payment = make_shared<Payment>(1, 25000);

  bool paymentStatus = rentalSystem.getPaymentService().makePayment(payment);

  if (paymentStatus) {
    cout << "Payment Successful" << endl;
  }

  return 0;
}
