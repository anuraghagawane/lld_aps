#pragma once
class Dice {
private:
  int minValue;
  int maxValue;

public:
  Dice(int minValue, int maxValue);

  int roll();
};
