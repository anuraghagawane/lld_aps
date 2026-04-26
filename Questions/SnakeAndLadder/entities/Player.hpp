#pragma once
#include <string>

class Player {
private:
  std::string name;
  int position;

public:
  Player(const std::string &names);

  std::string getName() const;

  int getPosition() const;

  void setPosition(int position);
};
