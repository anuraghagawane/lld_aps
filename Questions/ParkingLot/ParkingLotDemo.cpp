#include "ParkingLot.hpp"
#include "Vehicle.hpp"
#include <iostream>

using namespace std;

int main() {
  ParkingLot parkingLot(2, 3, 2);

  std::cout << "Initial parking lot status:" << std::endl;
  parkingLot.displayInfo();

  Vehicle *car1 = new Vehicle("CAR001", VehicleType::CAR, "Red");
  Vehicle *car2 = new Vehicle("CAR002", VehicleType::CAR, "Blue");
  Vehicle *motorcycle =
      new Vehicle("MOTO001", VehicleType::MOTORCYCLE, "Black");
  Vehicle *truck = new Vehicle("TRUCK001", VehicleType::TRUCK, "White");

  std::cout << "\nParking vehicles..." << std::endl;

  if (parkingLot.parkVehicle(car1)) {
    std::cout << "Parked successfully: ";
    car1->displayInfo();
  }

  if (parkingLot.parkVehicle(motorcycle)) {
    std::cout << "Parked successfully: ";
    car1->displayInfo();
  }

  if (parkingLot.parkVehicle(truck)) {
    std::cout << "Parked successfully: ";
    truck->displayInfo();
  }

  parkingLot.displayInfo();
  parkingLot.displayOccupancy();

  std::cout << "\nRemoving vehicle CAR001..." << std::endl;

  Vehicle *removed = parkingLot.removeVehilce("CAR001");
  if (removed) {
    std::cout << "Removed successfully: ";
    removed->displayInfo();
    delete removed;
  }

  if (parkingLot.parkVehicle(car2)) {
    std::cout << "Parked successfully: ";
    car2->displayInfo();
  }

  std::cout << "\nFinal parking lot status:" << std::endl;
  parkingLot.displayInfo();
  parkingLot.displayOccupancy();

  delete car2;
  delete motorcycle;
  delete truck;

  return 0;
}
