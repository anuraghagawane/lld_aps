#include <iostream>
#include <string>

using namespace std;

// ------------------ Supporting Classes ------------------

class Profile {
private:
    string fullName;

public:
    Profile(string name) : fullName(name) {}

    string getFullName() const {
        return fullName;
    }
};

class Registration {
private:
    string licensePlate;

public:
    Registration(string plate) : licensePlate(plate) {}

    string getLicensePlate() const {
        return licensePlate;
    }
};

class Vehicle {
private:
    Registration registration;

public:
    Vehicle(const Registration& reg) : registration(reg) {}

    Registration getRegistration() const {
        return registration;
    }
};

class ContactInfo {
private:
    string phoneNumber;

public:
    ContactInfo(string phone) : phoneNumber(phone) {}

    string getPhoneNumber() const {
        return phoneNumber;
    }
};

// ------------------ Main Domain Classes ------------------

class Driver {
private:
    Profile profile;
    Vehicle vehicle;

public:
    Driver(Profile p, Vehicle v) : profile(p), vehicle(v) {}

    Profile getProfile() const {
        return profile;
    }

    Vehicle getVehicle() const {
        return vehicle;
    }
};

class Passenger {
private:
    ContactInfo contactInfo;

public:
    Passenger(ContactInfo c) : contactInfo(c) {}

    ContactInfo getContactInfo() const {
        return contactInfo;
    }
};

class Ride {
private:
    Driver driver;
    Passenger passenger;

public:
    Ride(Driver d, Passenger p) : driver(d), passenger(p) {}

    Driver getDriver() const {
        return driver;
    }

    Passenger getPassenger() const {
        return passenger;
    }

    // add following three function to follow law of demeter
    string getDriverName() const {
      return driver.getProfile().getFullName();
    }

    string getVehiclePlate() const {
        return driver.getVehicle().getRegistration().getLicensePlate();
    }

    string getPassengerPhone() const {
        return passenger.getContactInfo().getPhoneNumber();
    }
};

// ------------------ Notification Service ------------------

class NotificationService {
public:
    void sendRideUpdate(const Ride& ride) {

      // // Train wreck 1
      // string driverName = ride.getDriver()
      //                         .getProfile()
      //                         .getFullName();
      //
      // // Train wreck 2
      // string plate = ride.getDriver()
      //                    .getVehicle()
      //                    .getRegistration()
      //                    .getLicensePlate();
      //
      // // Train wreck 3
      // string phone = ride.getPassenger()
      //                    .getContactInfo()
      //                    .getPhoneNumber();
      //

      string driverName = ride.getDriverName();
      string plate = ride.getVehiclePlate();
      string phone = ride.getPassengerPhone();
      cout << "SMS to " << phone << ": Your driver " << driverName
            << " is arriving in a " << plate
            << ". Contact: " << phone << endl;
    }
};

// ------------------ Main ------------------

int main() {

    Profile profile("Rahul S");
    Registration reg("MH12AB1234");
    Vehicle vehicle(reg);
    ContactInfo contact("9876543210");

    Driver driver(profile, vehicle);
    Passenger passenger(contact);

    Ride ride(driver, passenger);

    NotificationService service;
    service.sendRideUpdate(ride);

    return 0;
}
