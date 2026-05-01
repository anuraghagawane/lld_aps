#pragma once

#include "ParkingSlot.hpp"
#include "Vehicle.hpp"
#include <map>
#include <vector>

class ParkingLot {
private:
  std::vector<ParkingSpot *> spots;
  std::map<std::string, ParkingSpot *> occupiedSpots;
  int capacity;
  int availableSpots;

public:
  ParkingLot(int numCompact, int numRegular, int numLarge);
  ~ParkingLot();

  int getCapacity() const;
  int getAvailableSpots() const;

  bool parkVehicle(Vehicle *vehicle);
  Vehicle *removeVehilce(const std::string &licensePlate);
  ParkingSpot *findVehicle(const std::string &licensePlate) const;

  void displayInfo() const;
  void displayOccupancy() const;

private:
  ParkingSpot *findAvailableSpot(const Vehicle *vehicle) const;
};
