#include "Board.hpp"

Board::Board(int size, const std::vector<BoardEntity *> &entities)
    : size(size) {
  for (BoardEntity *entity : entities) {
    snakeAndLadders[entity->getStart()] = entity->getEnd();
  }
}

int Board::getSize() const { return size; }

int Board::getFinalPosition(int position) const {
  auto it = snakeAndLadders.find(position);
  return it != snakeAndLadders.end() ? it->second : position;
}
