#include <atomic>
#include <chrono>
#include <ctime>
#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <unordered_map>
#include <vector>

using namespace std;

typedef chrono::system_clock::time_point tp;
enum class SeatType { STANDARD, PREMIUM };
enum class SeatStatus { AVAILABLE, RESERVED, BOOKED };

class Movie {
private:
  string id;
  string name;
  int duration;

public:
  Movie(const string &id, const string &name, const int duration)
      : id(id), name(name), duration(duration) {}
  string getId() const { return id; }

  string getName() const { return name; }

  int getDuration() const { return duration; }
};

class Seat {
private:
  string id;
  SeatType type;

public:
  Seat(const string &id, SeatType type) : id(id), type(type) {}

  string getId() const { return id; }
  SeatType getSeatType() const { return type; }
};

class ShowSeat {
private:
  shared_ptr<Seat> seat;
  SeatStatus status;
  string bookingId;

public:
  ShowSeat(shared_ptr<Seat> seat) : seat(seat), status(SeatStatus::AVAILABLE) {}

  string getId() const { return seat->getId(); }
  SeatType getSeatType() const { return seat->getSeatType(); }
  SeatStatus getSeatStatus() const { return status; }

  void reserve(const string &bookingId) {
    status = SeatStatus::RESERVED;
    this->bookingId = bookingId;
  }
  void confirm(const string &bookingId) { status = SeatStatus::BOOKED; }
  void release(const string &bookingId) {
    if (this->bookingId == bookingId) {
      status = SeatStatus::AVAILABLE;
    }
  }
};

class Hall {
private:
  string id;
  unordered_map<string, shared_ptr<Seat>> seats;

public:
  Hall(const string &id) : id(id) {}
  string getId() const { return id; }

  vector<shared_ptr<Seat>> getAllSeat() {
    vector<shared_ptr<Seat>> res;
    for (auto &[key, seat] : seats) {
      res.push_back(seat);
    }

    return res;
  }

  void addSeat(const string &id, SeatType type) {
    seats[id] = make_shared<Seat>(id, type);
  }
};

class Theatre {
private:
  string id;
  string name;
  unordered_map<string, unique_ptr<Hall>> halls;

public:
  Theatre(const string &id, const string &name) : id(id), name(name) {}

  string getId() const { return id; }
  string getName() const { return name; }

  void addHall(unique_ptr<Hall> hall) {
    const string id = hall->getId();
    halls[id] = std::move(hall);
  }
};

class Show {
private:
  string id;
  string movieId;
  string hallId;
  string timeSlot;
  unordered_map<string, unique_ptr<ShowSeat>> seats;
  unordered_map<SeatType, int> tierPrice;

  mutex mtx;

public:
  Show(const string &id, const string &movieId, const string &hallId,
       const string &timeSlot, unordered_map<SeatType, int> tierPrice,
       vector<shared_ptr<Seat>> seats)
      : id(id), movieId(movieId), hallId(hallId), timeSlot(timeSlot),
        tierPrice(tierPrice) {
    for (auto seat : seats) {
      this->seats[seat->getId()] = make_unique<ShowSeat>(seat);
    }
  }

  string getId() const { return id; }
  string getMovieId() const { return movieId; }
  string getHallId() const { return hallId; }
  string getTimeSlot() const { return timeSlot; }
  int getSeatPrice(SeatType type) { return tierPrice[type]; }
  SeatStatus getSeatStatus(const string &seatId) {
    if (!seats.count(seatId)) {
      throw runtime_error("Seat not present!");
    }
    return seats[seatId]->getSeatStatus();
  }

  int getTotalPrice(vector<string> &seatIds) {
    int total = 0;
    for (auto id : seatIds) {
      if (!seats.count(id)) {
        throw runtime_error("Seat not present!");
      }
      total += tierPrice[seats[id]->getSeatType()];
    }

    return total;
  }

  bool reserveSeats(const string &bookingId, vector<string> &seatIds) {
    lock_guard<mutex> lock(mtx);
    for (auto id : seatIds) {
      if (getSeatStatus(id) != SeatStatus::AVAILABLE) {
        cout << "Seat: " << id << " not available" << endl;
        return false;
      }
    }
    for (auto id : seatIds) {
      seats[id]->reserve(bookingId);
    }

    return true;
  }

  void releaseSeats(const string &bookingId, vector<string> &seatIds) {
    lock_guard<mutex> lock(mtx);
    for (auto id : seatIds) {
      seats[id]->release(bookingId);
    }
  }

  void confirmSeats(const string &bookingId, vector<string> &seatIds) {
    lock_guard<mutex> lock(mtx);
    for (auto id : seatIds) {
      seats[id]->confirm(bookingId);
    }
  }
};

enum class BookingStatus {
  PENDING_PAYMENT,
  CONFIRMED,
  PAYMENT_FAILED,
  EXPIRED,
  CANCELLED
};

class Booking {
private:
  string id;
  string userId;
  string showId;
  vector<string> seatIds;
  string paymentId;
  BookingStatus status;
  // int expireAt;
  tp expireAt;

public:
  Booking(const string &id, const string &userId, const string &showId,
          vector<string> seatIds, const string paymentId, const int expireAt)
      : id(id), userId(userId), showId(showId), seatIds(seatIds),
        paymentId(paymentId), status(BookingStatus::PENDING_PAYMENT),
        expireAt(chrono::system_clock::now() + chrono::seconds(120)) {}

  string getId() const { return id; }
  string getUserId() const { return userId; }
  string getShowId() const { return showId; }
  vector<string> getSeatIds() const { return seatIds; }
  string getPaymentId() const { return paymentId; }
  BookingStatus getBookingStatus() const { return status; }
  tp getExpiryTime() const { return expireAt; }

  bool setBookingStatus(BookingStatus status) {
    // we can handle properly, with determined state transitions
    this->status = status;
    return true;
  }
};

enum class PaymentStatus { PENDING, FAILED, SUCCESS };

class Payment {
private:
  string id;
  string bookingId;
  PaymentStatus status;
  int totalAmount;

public:
  Payment(const string &id, const string &bookingId, const int totalAmount)
      : id(id), bookingId(bookingId), totalAmount(totalAmount),
        status(PaymentStatus::PENDING) {}

  string getId() const { return id; }
  string getBookingId() const { return bookingId; }
  PaymentStatus getStatus() const { return status; }

  void sucess() { status = PaymentStatus::SUCCESS; }

  void fail() { status = PaymentStatus::FAILED; }
};

class CatalogService;
class PaymentService;

class BookingService {
private:
  unordered_map<string, unique_ptr<Booking>> bookings;
  CatalogService &catalogService;
  PaymentService &paymentService;
  friend class CatalogService;

  atomic<int> bookingCount = 1;

public:
  BookingService(CatalogService &catalogService, PaymentService &paymentService)
      : catalogService(catalogService), paymentService(paymentService) {}
  void book(string userId, string showId, vector<string> &seatIds);
};

class CatalogService {
private:
  unordered_map<string, shared_ptr<Show>> shows;
  unordered_map<string, shared_ptr<Movie>> movies;
  unordered_map<string, shared_ptr<Theatre>> theatres;

public:
  shared_ptr<Theatre> getTheatre(const string &theatreId) {
    if (!theatres.count(theatreId)) {
      throw runtime_error("Theatre not found");
    }
    return theatres[theatreId];
  }

  shared_ptr<Movie> getMovie(const string &movieId) {
    if (!movies.count(movieId)) {
      throw runtime_error("Movie not found");
    }
    return movies[movieId];
  }

  shared_ptr<Show> getShow(const string &showId) {
    if (!shows.count(showId)) {
      throw runtime_error("Show not found");
    }

    return shows[showId];
  }

  void addTheatre(shared_ptr<Theatre> theatre) {
    theatres[theatre->getId()] = theatre;
  }

  void addMovie(shared_ptr<Movie> movie) { movies[movie->getId()] = movie; }

  void addShow(shared_ptr<Show> show) { shows[show->getId()] = show; }
};

class PaymentService {
private:
  unordered_map<string, unique_ptr<Payment>> payments;
  atomic<int> count = 1;

public:
  string init(const string &bookingId, const int totalPrice) {
    string id = "PAY_" + to_string(count.fetch_add(1));
    auto payment = make_unique<Payment>(id, bookingId, totalPrice);
    payments[id] = std::move(payment);
    return id;
  }

  bool pay(const string &paymentId) {
    if (!payments.count(paymentId)) {
      throw runtime_error("Payment not found!");
    }
    auto &payment = payments[paymentId];

    // simulate payment

    payment->sucess();
    return payment->getStatus() == PaymentStatus::SUCCESS;
  }
};

void BookingService::book(string userId, string showId,
                          vector<string> &seatIds) {
  string bookingId = "BOOK_" + to_string(bookingCount.fetch_add(1));
  shared_ptr<Show> show = catalogService.getShow(showId);
  if (!show->reserveSeats(bookingId, seatIds)) {
    cout << "Failed to reserve seats. Choose different seats or try after "
            "some time"
         << endl;
    return;
  }

  int totalPrice = show->getTotalPrice(seatIds);

  string paymentId = paymentService.init(bookingId, totalPrice);
  auto booking =
      make_unique<Booking>(bookingId, userId, showId, seatIds, paymentId, 2);

  bookings[bookingId] = std::move(booking);
  if (!paymentService.pay(paymentId)) {
    cout << "Payment failed: " << paymentId << endl;
    auto show = catalogService.getShow(showId);
    show->releaseSeats(bookingId, seatIds);
    bookings[bookingId]->setBookingStatus(BookingStatus::PAYMENT_FAILED);
    return;
  }

  show->confirmSeats(bookingId, seatIds);

  bookings[bookingId]->setBookingStatus(BookingStatus::CONFIRMED);

  cout << "Booking completed: " << bookingId << " Payment id: " << paymentId
       << " Price: " << totalPrice << endl;
}
int main() {
  cout << "Movie Ticket Booking System" << endl;
  CatalogService catalogService;
  PaymentService paymentService;
  BookingService bookingService(catalogService, paymentService);
  auto theatre = make_shared<Theatre>("T1", "HPR");
  catalogService.addTheatre(theatre);
  auto movie = make_shared<Movie>("M1", "MOVIE 1", 180);
  catalogService.addMovie(movie);

  auto tierList = unordered_map<SeatType, int>{{SeatType::PREMIUM, 200},
                                               {SeatType::STANDARD, 100}};

  auto h1 = make_unique<Hall>("H1");
  h1->addSeat("S1", SeatType::STANDARD);
  h1->addSeat("S2", SeatType::STANDARD);
  h1->addSeat("S3", SeatType::STANDARD);
  h1->addSeat("S4", SeatType::STANDARD);
  h1->addSeat("P1", SeatType::PREMIUM);
  h1->addSeat("P2", SeatType::PREMIUM);

  auto show =
      make_shared<Show>("S1", "M1", "H1", "T1", tierList, h1->getAllSeat());

  theatre->addHall(std::move(h1));
  catalogService.addShow(show);

  vector<string> seatsToBook = {"S1", "S2"};
  bookingService.book("U1", "S1", seatsToBook);
  bookingService.book("U1", "S1", seatsToBook);
  return 0;
}
