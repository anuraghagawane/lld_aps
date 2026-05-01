#include "ParkingSlot.hpp"
#include "Vehicle.hpp"
#include "iostream"

ParkingSpot::ParkingSpot(int spotNumer, SpotType type)
    : spotNumber(spotNumer), type(type), vehicle(nullptr), available(true) {}

int ParkingSpot::getSpotNumber() const { return spotNumber; }
SpotType ParkingSpot::getType() const { return type; }
Vehicle *ParkingSpot::getVehicle() const { return vehicle; }
bool ParkingSpot::isAvailable() const { return available; }

bool ParkingSpot::canFitVehicle(const Vehicle *vehicle) const {
  if (!vehicle)
    return false;

  switch (vehicle->getType()) {
  case VehicleType::MOTORCYCLE:
    return true;
  case VehicleType::CAR:
    return type != SpotType::COMPACT;
  case VehicleType::TRUCK:
  case VehicleType::BUS:
    return type == SpotType::LARGE;
  }

  return false;
}

bool ParkingSpot::parkVehicle(Vehicle *vehicle) {
  if (!available || !canFitVehicle(vehicle))
    return false;

  this->vehicle = vehicle;
  available = false;
  return true;
}

Vehicle *ParkingSpot::removeVehicle() {
  if (!vehicle)
    return nullptr;

  Vehicle *removedVehicle = vehicle;
  vehicle = nullptr;
  available = true;

  return removedVehicle;
}

void ParkingSpot::displayInfo() const {
  std::cout << "Spot " << spotNumber << " (";
  switch (type) {
  case SpotType::COMPACT:
    std::cout << "Compact";
    break;
  case SpotType::REGULAR:
    std::cout << "Regular";
    break;
  case SpotType::LARGE:
    std::cout << "Large";
    break;
  }
  std::cout << "): " << (available ? "Available" : "Occupied");
  if (vehicle) {
    std::cout << " by ";
    vehicle->displayInfo();
  } else {
    std::cout << std::endl;
  }
}
