#include <atomic>
#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

enum class SeatStatus { AVAILABLE, HELD, BOOKED };

enum class BookingStatus {
  PAYMENT_PENDING,
  BOOKED,
  FAILED,
  CANCELLED,
  REFUNDED
};

class User {
private:
  string id;
  string name;
  string contactNumber;
  string email;

public:
  User(const string &id, const string &name, const string &contactNumber,
       const string &email)
      : id(id), name(name), contactNumber(contactNumber), email(email) {}

  string getId() const { return id; }
  string getName() const { return name; }
  string getEmail() const { return email; }
  string getContactNumber() const { return contactNumber; }
};

class Venue {
private:
  string id;
  string name;
  string address;

public:
  Venue(const string &id, const string &name, const string &address)
      : id(id), name(name), address(address) {}

  string getId() const { return id; }
  string getName() const { return name; }
  string getaddress() const { return address; }
};

class Seat {
private:
  string id;
  SeatStatus status;

  mutable mutex mtx;

public:
  Seat(const string &id) : id(id), status(SeatStatus::AVAILABLE) {}
  string getId() const { return id; }

  SeatStatus getStatus() const { return status; }

  bool isAvailable() const { return status == SeatStatus::AVAILABLE; }

  void hold() {
    lock_guard<mutex> lock(mtx);
    if (!isAvailable()) {
      throw runtime_error("Seat is not available");
    }
    status = SeatStatus::HELD;
  }

  void confirm() {
    lock_guard<mutex> lock(mtx);
    status = SeatStatus::BOOKED;
  }

  void release() {
    lock_guard<mutex> lock(mtx);
    status = SeatStatus::AVAILABLE;
  }
};

class Booking {
private:
  string id;
  string userId;
  string concertId;
  vector<string> seatIds;
  BookingStatus status;

public:
  Booking(const string &id, const string &userId, const string &concertId,
          const vector<string> seatIds)
      : id(id), userId(userId), concertId(concertId), seatIds(seatIds),
        status(BookingStatus::PAYMENT_PENDING) {}

  string getId() const { return id; }
  string getConcertId() const { return concertId; }
  vector<string> getSeatIds() const { return seatIds; }

  void confirm() { status = BookingStatus::BOOKED; }

  void fail() { status = BookingStatus::FAILED; }

  void cancel() { status = BookingStatus::CANCELLED; }

  void refund() { status = BookingStatus::REFUNDED; }
};

class Concert {
private:
  string id;
  string artistName;
  string date;
  shared_ptr<Venue> venue;
  unordered_map<string, unique_ptr<Seat>> seats;
  int seatCost;

  mutable mutex mtx;

public:
  Concert(const string &id, const string &artistName, const string &date,
          shared_ptr<Venue> venue, int seatCost, int seatCount)
      : id(id), artistName(artistName), date(date), venue(venue),
        seatCost(seatCost) {
    while (seatCount--) {
      auto seat = make_unique<Seat>("S_" + to_string(seatCount));
      seats[seat->getId()] = std::move(seat);
    }
  }

  void showAvailableSeats() {
    cout << "Available Seats: " << endl;
    for (auto &[id, seat] : seats) {
      if (seat->isAvailable()) {
        cout << " - " << seat->getId() << endl;
      }
    }
  }

  string getId() const { return id; }

  string getArtistName() const { return artistName; }

  string getVenueName() const { return venue->getName(); }

  string getDate() const { return date; }

  int getPerSeatCost() const { return seatCost; }

  bool areSeatsAvailable(vector<string> seatIds) {
    for (auto &seatId : seatIds) {
      if (!seats.count(seatId) || !seats[seatId]->isAvailable()) {
        cout << "Seat: " << seatId << " is not available";
        return false;
      }
    }

    return true;
  }

  void holdSeats(shared_ptr<Booking> booking) {
    lock_guard<mutex> lock(mtx);
    if (!areSeatsAvailable(booking->getSeatIds())) {
      return;
    }

    for (auto &seatId : booking->getSeatIds()) {
      seats[seatId]->hold();
    }
  }

  void confirmSeats(shared_ptr<Booking> booking) {
    lock_guard<mutex> lock(mtx);

    for (auto &seatId : booking->getSeatIds()) {
      seats[seatId]->confirm();
    }
  }

  void releaseSeats(shared_ptr<Booking> booking) {
    lock_guard<mutex> lock(mtx);

    for (auto &seatId : booking->getSeatIds()) {
      seats[seatId]->release();
    }
  }
};

// class PaymentStrategy;

enum class PaymentStatus { PENDING, DONE };
class Payment {
private:
  string id;
  const int amount;
  PaymentStatus status;

public:
  Payment(const string &id, const int amount)
      : id(id), amount(amount), status(PaymentStatus::PENDING) {}

  // can be improved using strategy to support different methods
  bool pay() {
    status = PaymentStatus::DONE;
    return true;
  }
};

class NotificationService {
public:
  void notify(const User *user, const shared_ptr<Booking> booking) {
    // can be improved to strategy
    sendSms(user->getContactNumber(),
            "Booking Confirmed for id: " + booking->getId());
    sendEmail(user->getEmail(),
              "Booking Confirmed for id: " + booking->getId());
  }

private:
  void sendSms(string to, string content) {
    cout << "Sending SMS: " << endl;
    cout << "To: " << to << endl;
    cout << "Content: " << content << endl;
  }

  void sendEmail(string to, string content) {
    cout << "Sending EMAIL: " << endl;
    cout << "To: " << to << endl;
    cout << "Content: " << content << endl;
  }
};

class BookingService {
private:
  unordered_map<string, unique_ptr<Concert>> concerts;
  unordered_map<string, shared_ptr<Booking>> bookings;
  unordered_map<string, unique_ptr<User>> users;

  atomic<int> bookingCounter{1};

  NotificationService notificationService;

  BookingService() {};

public:
  BookingService(const BookingService &) = delete;
  BookingService &operator=(const BookingService &) = delete;
  static BookingService &getInstance() {
    static BookingService service;
    return service;
  }
  string createUser(const string &id, const string &name,
                    const string &contactNumber, const string &email) {
    auto user = make_unique<User>(id, name, contactNumber, email);
    users[user->getId()] = std::move(user);

    return id;
  }

  string addConcert(const string &id, const string &artistName,
                    const string &date, shared_ptr<Venue> venue, int seatCost,
                    int seatCount) {
    auto concert =
        make_unique<Concert>(id, artistName, date, venue, seatCost, seatCount);
    concerts[id] = std::move(concert);

    return id;
  }

  void searchConcert(const string &artistName, const string &venueName,
                     const string &date) {
    cout << "Concerts: " << endl;
    for (auto &[id, concert] : concerts) {
      if (concert->getArtistName() == artistName &&
          concert->getVenueName() == venueName && concert->getDate() == date) {
        cout << " " << concert->getId() << endl;
      }
    }
  }

  void showAvailableSeats(const string &concertId) {
    if (!concerts.count(concertId)) {
      throw runtime_error("Invalid concertId");
    }
    concerts[concertId]->showAvailableSeats();
  }

  string book(const string &concertId, const string &userId,
              const vector<string> seatIds) {
    if (!concerts.count(concertId)) {
      throw runtime_error("Invalid concertId");
    }

    auto concert = concerts[concertId].get();

    if (!users.count(userId)) {
      throw runtime_error("Invalid User");
    }

    auto user = users[userId].get();

    if (!concert->areSeatsAvailable(seatIds)) {
      throw runtime_error("Seats not available");
    }

    const string bookingId = "B_" + to_string(bookingCounter.fetch_add(1));
    auto booking = make_shared<Booking>(bookingId, userId, concertId, seatIds);
    concert->holdSeats(booking);
    Payment payment("P_" + bookingId,
                    booking->getSeatIds().size() * concert->getPerSeatCost());

    bool isSuccess = payment.pay();
    if (!isSuccess) {
      concert->releaseSeats(booking);
      booking->fail();
      throw runtime_error("Payment failed");
    }
    concert->confirmSeats(booking);

    bookings[bookingId] = booking;

    notificationService.notify(user, booking);

    return bookingId;
  }

  void cancelBooking(const string &bookingId) {
    if (!bookings.count(bookingId)) {
      throw runtime_error("No booking found");
    }

    auto booking = bookings[bookingId];
    const string concertId = booking->getConcertId();
    concerts[concertId]->releaseSeats(booking);
    booking->refund();
    cout << "Cancel successfull for BookingID: " << bookingId << endl;
  }
};

int main() {
  BookingService &bookingService = BookingService::getInstance();
  auto u1 = bookingService.createUser("u1", "Anurag", "9191919", "a@b.com");

  auto venue1 = make_shared<Venue>("V1", "TAJ", "A street");
  auto c1 =
      bookingService.addConcert("C1", "CJ", "2026-06-13", venue1, 500, 10);

  bookingService.searchConcert("CJ", "TAJ", "2026-06-13");

  bookingService.showAvailableSeats("C1");

  auto b1 = bookingService.book("C1", "u1", {"S_0", "S_1", "S_2"});

  bookingService.showAvailableSeats("C1");

  bookingService.cancelBooking(b1);

  bookingService.showAvailableSeats("C1");
  return 0;
}
