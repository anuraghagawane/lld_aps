#ifndef CELL_HPP
#define CELL_HPP

#include "../enums/enums.hpp"
class Cell {
private:
  Symbol symbol_;

public:
  Cell();
  Symbol getSymbol() const;
  void setSymbol(Symbol symbol);
  bool isEmpty() const;
};

#endif // !DEBUG
