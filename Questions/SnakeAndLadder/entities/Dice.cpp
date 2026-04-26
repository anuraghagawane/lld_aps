#include "Dice.hpp"
#include <random>

Dice::Dice(int minValue, int maxValue)
    : minValue(minValue), maxValue(maxValue) {}

int Dice::roll() {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(minValue, maxValue);
  return dis(gen);
}
