#pragma once

#include <string>

enum class VehicleType { CAR, MOTORCYCLE, TRUCK, BUS };

class Vehicle {
private:
  std::string licensePlate;
  VehicleType type;
  std::string color;

public:
  Vehicle(std::string licensePlate, VehicleType type, std::string color);
  std::string getLicensePlate() const;
  VehicleType getType() const;
  std::string getColor() const;
  void displayInfo() const;
};
