#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

enum class SeatStatus { AVAILABLE, BOOKED };

enum class BookingStatus {
  CONFIRMED,
  CANCELLED,
};

class Passenger {
private:
  string id;
  string name;

public:
  Passenger(const string &id, const string &name) : id(id), name(name) {}

  string getId() const { return id; }

  string getName() const { return name; }
};

class Seat {
private:
  string seatNumber;
  SeatStatus status;

  mutable mutex mtx;

public:
  Seat(const string &seatNumber)
      : seatNumber(seatNumber), status(SeatStatus::AVAILABLE) {}

  bool isAvailable() {
    lock_guard<mutex> lock(mtx);
    return status == SeatStatus::AVAILABLE;
  }

  bool book() {
    lock_guard<mutex> lock(mtx);
    if (SeatStatus::BOOKED == status)
      return false;
    status = SeatStatus::BOOKED;
    return true;
  }

  void release() {
    lock_guard<mutex> lock(mtx);
    status = SeatStatus::AVAILABLE;
  }
};

class Flight {
private:
  string flightNumber;
  string source;
  string destination;

public:
  Flight(const string &flightNumber, const string &source,
         const string &destination)
      : flightNumber(flightNumber), source(source), destination(destination) {}

  string getFlightNumber() const { return flightNumber; }

  string getSource() const { return source; }

  string getDestination() const { return destination; }
};

class FlightInstance {
private:
  string id;
  shared_ptr<Flight> flight;
  string date;

  unordered_map<string, shared_ptr<Seat>> seats;

public:
  FlightInstance(const string &id, shared_ptr<Flight> flight,
                 const string &date)
      : id(id), flight(flight), date(date) {}
  string getId() const { return id; }
  string getSource() const { return flight->getSource(); }
  string getDestination() const { return flight->getDestination(); }
  string getDate() const { return date; }

  shared_ptr<Seat> getSeat(const string &seatNo) {
    if (!seats.count(seatNo)) {
      return nullptr;
    }

    return seats[seatNo];
  }

  void showAvailableSeats() {
    cout << "\nAvailable Seats:\n";

    for (auto &[seatNo, seat] : seats) {
      if (seat->isAvailable()) {
        cout << seatNo << " ";
      }
    }

    cout << endl;
  }

  void addSeat(const string &seatNumber) {
    seats[seatNumber] = make_shared<Seat>(seatNumber);
  }
};

class Booking {
private:
  string id;
  shared_ptr<Passenger> passenger;
  shared_ptr<FlightInstance> flightInstance;
  string seatNumber;
  BookingStatus status;

public:
  Booking(const string &id, shared_ptr<Passenger> passenger,
          shared_ptr<FlightInstance> flightInstance, const string &seatNumber)
      : id(id), passenger(passenger), flightInstance(flightInstance),
        seatNumber(seatNumber), status(BookingStatus::CONFIRMED) {}

  string getId() const { return id; }

  string getSeatNumber() const { return seatNumber; }

  shared_ptr<FlightInstance> getFlightInstance() const {
    return flightInstance;
  }

  BookingStatus getStatus() const { return status; }

  void cancel() { status = BookingStatus::CANCELLED; }
};

class BookingService {
private:
  unordered_map<string, shared_ptr<FlightInstance>> flights;
  unordered_map<string, shared_ptr<Booking>> bookings;

  int bookingCounter = 1;

public:
  void addFlightInstance(shared_ptr<FlightInstance> flight) {
    flights[flight->getId()] = flight;
  }

  vector<shared_ptr<FlightInstance>> searchFlights(const string &source,
                                                   const string &destination,
                                                   const string &date) {
    vector<shared_ptr<FlightInstance>> result;

    for (auto &[id, instance] : flights) {
      if (instance->getSource() == source &&
          instance->getDestination() == destination &&
          instance->getDate() == date) {
        result.push_back(instance);
      }
    }

    return result;
  }

  shared_ptr<Booking> createBooking(shared_ptr<Passenger> passenger,
                                    const string &flightInstanceId,
                                    const string &seatNumber) {
    if (!flights.count(flightInstanceId)) {
      throw runtime_error("Flight not found");
    }
    auto flightInstance = flights[flightInstanceId];
    auto seat = flightInstance->getSeat(seatNumber);
    if (!seat) {
      throw runtime_error("Seat not found");
    }

    if (!seat->book()) {
      throw runtime_error("Seat already book");
    }
    string bookingId = "BOOK_" + to_string(bookingCounter++);
    auto booking =
        make_shared<Booking>(bookingId, passenger, flightInstance, seatNumber);

    bookings[bookingId] = booking;

    cout << "\nBooking Successful\n";
    cout << "Booking ID: " << bookingId << endl;

    return booking;
  }

  void cancelBooking(const string &bookingId) {
    if (!bookings.count(bookingId)) {
      throw runtime_error("Booking not found");
    }

    auto booking = bookings[bookingId];

    if (booking->getStatus() == BookingStatus::CANCELLED) {
      return;
    }

    auto seat = booking->getFlightInstance()->getSeat(booking->getSeatNumber());

    seat->release();

    booking->cancel();
    cout << "\nBooking Cancelled: " << bookingId << endl;
  }
};

int main() {
  BookingService bookingService;

  auto flight = make_shared<Flight>("AI101", "Mumbai", "Delhi");

  auto instance = make_shared<FlightInstance>("FI1", flight, "2026-06-20");

  instance->addSeat("1A");
  instance->addSeat("1B");
  instance->addSeat("1C");

  bookingService.addFlightInstance(instance);

  auto passenger1 = make_shared<Passenger>("P1", "Anurag");

  auto passenger2 = make_shared<Passenger>("P2", "Rahul");

  auto flights = bookingService.searchFlights("Mumbai", "Delhi", "2026-06-20");

  cout << "Flights Found: " << flights.size() << endl;

  instance->showAvailableSeats();

  try {

    auto booking1 = bookingService.createBooking(passenger1, "FI1", "1A");

    instance->showAvailableSeats();

    bookingService.cancelBooking(booking1->getId());

    instance->showAvailableSeats();

  } catch (const exception &e) {

    cout << e.what() << endl;
  }
  return 0;
}
