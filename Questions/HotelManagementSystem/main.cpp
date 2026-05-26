#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <unordered_map>
#include <vector>

using namespace std;

enum class RoomType { SINGLE, DOUBLE, DELUXE, SUITE };

enum class RoomStatus { AVAILABLE, RESERVED, OCCUPIED, MAINTENANCE };

enum class ReservationStatus {
  CREATED,
  CONFIRMED,
  CHECKED_IN,
  CHECKED_OUT,
  CANCELLED,
};

enum class PaymentStatus { PENDING, SUCCESS, FAILED };

string roomTypeToString(RoomType type) {
  switch (type) {
  case RoomType::SINGLE:
    return "SINGLE";
  case RoomType::DOUBLE:
    return "DOUBLE";
  case RoomType::DELUXE:
    return "DELUXE";
  case RoomType::SUITE:
    return "SUITE";
  }

  return "";
}

bool isDateOverlap(int start1, int end1, int start2, int end2) {
  return !(end1 <= start2 || start1 >= end2);
}

class Guest {
private:
  string id;
  string name;

public:
  Guest(const string &id, const string &name) : id(id), name(name) {}

  string getId() const { return id; }
  string getName() const { return name; }
};

class Room {
private:
  string id;
  int roomNumber;
  RoomType type;
  double pricePerNight;
  RoomStatus status;

  mutable mutex mtx;

public:
  Room(const string &id, int roomNumber, RoomType type, double pricePerNight)
      : id(id), roomNumber(roomNumber), type(type),
        pricePerNight(pricePerNight), status(RoomStatus::AVAILABLE) {}

  string getId() const { return id; }

  int getRoomNumber() const { return roomNumber; }

  RoomType getType() const { return type; }

  double getPricePerNight() const { return pricePerNight; }

  RoomStatus getStatus() const {
    lock_guard<mutex> lock(mtx);
    return status;
  }

  void setStatus(RoomStatus newStatus) {
    lock_guard<mutex> lock(mtx);
    status = newStatus;
  }
};

class Reservation {
private:
  string id;
  shared_ptr<Guest> guest;
  shared_ptr<Room> room;
  int checkInDate;
  int checkOutDate;
  ReservationStatus status;
  double totalAmount;

public:
  Reservation(const string &id, shared_ptr<Guest> guest, shared_ptr<Room> room,
              int checkInDate, int checkOutDate)
      : id(id), guest(guest), room(room), checkInDate(checkInDate),
        checkOutDate(checkOutDate), status(ReservationStatus::CONFIRMED) {
    int totalDays = checkOutDate - checkInDate;
    totalAmount = totalDays * room->getPricePerNight();
  }

  string getId() const { return id; }
  shared_ptr<Guest> getGuest() const { return guest; }
  shared_ptr<Room> getRoom() const { return room; }
  int getCheckInDate() const { return checkInDate; }
  int getCheckOutDate() const { return checkOutDate; }
  double getTotalAmount() const { return totalAmount; }
  ReservationStatus getStatus() const { return status; }
  void setStatus(ReservationStatus s) { status = s; }
};

class PaymentStrategy {
public:
  virtual void pay(double amount) = 0;
  virtual ~PaymentStrategy() = default;
};

class CashPayment : public PaymentStrategy {
public:
  void pay(double amount) override {
    cout << "Cash Payment Successful => " << amount << endl;
  }
};

class CardPayment : public PaymentStrategy {
public:
  void pay(double amount) override {
    cout << "Card Payment Successful => " << amount << endl;
  }
};

class OnlinePayment : public PaymentStrategy {
public:
  void pay(double amount) override {
    cout << "Online Payment Successful => " << amount << endl;
  }
};

class Invoice {
private:
  string id;
  shared_ptr<Reservation> reservation;
  double taxes;
  double serviceCharges;
  double totalAmount;

  PaymentStatus paymentStatus;

public:
  Invoice(const string &id, shared_ptr<Reservation> reservation, double taxes,
          double serviceCharges)
      : id(id), reservation(reservation), taxes(taxes),
        serviceCharges(serviceCharges), paymentStatus(PaymentStatus::PENDING) {
    totalAmount = reservation->getTotalAmount() + taxes + serviceCharges;
  }

  double getTotalAmount() const { return totalAmount; }

  void markPaid() { paymentStatus = PaymentStatus::SUCCESS; }
};

class RoomService {
private:
  unordered_map<string, shared_ptr<Room>> rooms;
  mutable mutex mtx;

public:
  void addRoom(shared_ptr<Room> room) {
    lock_guard<mutex> lock(mtx);
    rooms[room->getId()] = room;
  }

  vector<shared_ptr<Room>> searchAvailableRooms(RoomType type) {
    lock_guard<mutex> lock(mtx);

    vector<shared_ptr<Room>> result;

    for (auto &[id, room] : rooms) {
      if (room->getType() == type &&
          room->getStatus() == RoomStatus::AVAILABLE) {
        result.push_back(room);
      }
    }

    return result;
  }

  shared_ptr<Room> getRoom(const string &roomId) {
    lock_guard<mutex> lock(mtx);
    if (!rooms.count(roomId)) {
      return nullptr;
    }

    return rooms[roomId];
  }
};

class ReservationService {
private:
  unordered_map<string, shared_ptr<Reservation>> reservations;
  mutable mutex mtx;

public:
  bool isRoomAvailable(shared_ptr<Room> room, int checkIn, int checkOut) {
    for (auto &[id, reservation] : reservations) {
      if (reservation->getRoom()->getId() != room->getId()) {
        continue;
      }

      if (reservation->getStatus() == ReservationStatus::CANCELLED ||
          reservation->getStatus() == ReservationStatus::CHECKED_OUT) {
        continue;
      }

      if (isDateOverlap(checkIn, checkOut, reservation->getCheckInDate(),
                        reservation->getCheckOutDate())) {
        return false;
      }
    }

    return true;
  }

  shared_ptr<Reservation> createReservation(const string &reservationId,
                                            shared_ptr<Guest> guest,
                                            shared_ptr<Room> room, int checkIn,
                                            int checkOut) {
    lock_guard<mutex> lock(mtx);

    if (!isRoomAvailable(room, checkIn, checkOut)) {
      throw runtime_error("Room already booked for selected dates");
    }

    auto reservation =
        make_shared<Reservation>(reservationId, guest, room, checkIn, checkOut);

    reservations[reservationId] = reservation;

    room->setStatus(RoomStatus::RESERVED);

    cout << "Reservation created => " << reservationId << endl;

    return reservation;
  }

  void checkIn(const string &reservationId) {
    lock_guard<mutex> lock(mtx);

    if (!reservations.count(reservationId)) {
      throw runtime_error("Reservation not found");
    }

    auto reservation = reservations[reservationId];

    reservation->setStatus(ReservationStatus::CHECKED_IN);

    reservation->getRoom()->setStatus(RoomStatus::OCCUPIED);

    cout << "Guest Checked In => " << reservation->getGuest()->getName()
         << endl;
  }

  void checkOut(const string &reservationId) {
    lock_guard<mutex> lock(mtx);

    if (!reservations.count(reservationId)) {
      throw runtime_error("Reservation not found");
    }

    auto reservation = reservations[reservationId];

    reservation->setStatus(ReservationStatus::CHECKED_OUT);

    reservation->getRoom()->setStatus(RoomStatus::AVAILABLE);

    cout << "Guest Checked Out => " << reservation->getGuest()->getName()
         << endl;
  }
};

class BillingService {
public:
  shared_ptr<Invoice> generateInvoice(const string &invoiceId,
                                      shared_ptr<Reservation> reservation) {
    double taxes = reservation->getTotalAmount() * 0.18;
    double serviceCharges = 500;

    auto invoice =
        make_shared<Invoice>(invoiceId, reservation, taxes, serviceCharges);

    cout << "Invoice Generated => Total Amount: " << invoice->getTotalAmount()
         << endl;

    return invoice;
  }
};

class HotelManagementService {
private:
  RoomService roomService;
  ReservationService reservationService;
  BillingService billingService;

public:
  void addRoom(shared_ptr<Room> room) { roomService.addRoom(room); }

  vector<shared_ptr<Room>> searchRooms(RoomType type) {
    return roomService.searchAvailableRooms(type);
  }

  shared_ptr<Reservation> bookRoom(const string &reservationId,
                                   shared_ptr<Guest> guest,
                                   const string &roomId, int checkIn,
                                   int checkOut) {
    auto room = roomService.getRoom(roomId);

    if (!room) {
      throw runtime_error("Room not found");
    }

    return reservationService.createReservation(reservationId, guest, room,
                                                checkIn, checkOut);
  }

  void checkIn(const string &reservationId) {
    reservationService.checkIn(reservationId);
  }

  void checkOut(const string &reservationId,
                shared_ptr<Reservation> reservation,
                unique_ptr<PaymentStrategy> paymentStrategy) {
    auto invoice =
        billingService.generateInvoice("INV-" + reservationId, reservation);

    paymentStrategy->pay(invoice->getTotalAmount());
    invoice->markPaid();
    reservationService.checkOut(reservationId);
  }
};

int main() {
  HotelManagementService hotel;
  hotel.addRoom(make_shared<Room>("R1", 101, RoomType::SINGLE, 3000));
  hotel.addRoom(make_shared<Room>("R2", 102, RoomType::DOUBLE, 5000));
  hotel.addRoom(make_shared<Room>("R1", 101, RoomType::DELUXE, 8000));

  auto guest1 = make_shared<Guest>("G1", "Anurag");

  try {
    auto availableRooms = hotel.searchRooms(RoomType::DOUBLE);

    cout << "\nAvailable DOUBLE Rooms:\n";

    for (auto &room : availableRooms) {
      cout << "Room Number: " << room->getRoomNumber()
           << " Price: " << room->getPricePerNight() << endl;
    }

    auto reservation = hotel.bookRoom("RES-1", guest1, "R2", 10, 15);

    hotel.checkIn("RES-1");
    hotel.checkOut("RES-1", reservation, make_unique<CardPayment>());
  } catch (const exception &e) {
    cout << "Error: " << e.what() << endl;
  }
  return 0;
}
