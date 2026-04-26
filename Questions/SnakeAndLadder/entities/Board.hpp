#pragma once
#include "BoardEntity.hpp"
#include <map>
#include <vector>

class Board {
private:
  int size;
  std::pmr::map<int, int> snakeAndLadders;

public:
  Board(int size, const std::vector<BoardEntity *> &entities);
  int getSize() const;
  int getFinalPosition(int position) const;
};
