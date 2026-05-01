#include "Vehicle.hpp"
#include "iostream"

Vehicle::Vehicle(std::string licensePlate, VehicleType type, std::string color)
    : licensePlate(licensePlate), type(type), color(color) {}

std::string Vehicle::getLicensePlate() const { return licensePlate; }
VehicleType Vehicle::getType() const { return type; }
std::string Vehicle::getColor() const { return color; }

void Vehicle::displayInfo() const {
  std::cout << "Vehicle: " << color << " ";
  switch (type) {
  case VehicleType::CAR:
    std::cout << "CAR";
    break;
  case VehicleType::MOTORCYCLE:
    std::cout << "MOTORCYCLE";
    break;
  case VehicleType::TRUCK:
    std::cout << "TRUCK";
    break;
  case VehicleType::BUS:
    std::cout << "BUS";
    break;
  }

  std::cout << " (License: " << licensePlate << ")" << std::endl;
}
