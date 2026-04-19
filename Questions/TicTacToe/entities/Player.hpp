#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "../enums/enums.hpp"
#include <string>

class Player {
private:
  const std::string name_;
  const Symbol symbol_;

public:
  Player(const std::string &name, Symbol symbol);

  const std::string &getName() const;
  Symbol getSymbol() const;
  std::string toString() const;
};

#endif // !PLAYER_HPP
