#include "BoardEntity.hpp"
#include <stdexcept>

BoardEntity::BoardEntity(int start, int end) : start(start), end(end) {}

int BoardEntity::getStart() const { return start; }

int BoardEntity::getEnd() const { return end; }

Ladder::Ladder(int start, int end) : BoardEntity(start, end) {
  if (start >= end) {
    throw std::invalid_argument(
        "Ladder bottom must be at a lower position than its top.");
  }
}

Snake::Snake(int start, int end) : BoardEntity(start, end) {
  if (start <= end) {
    throw std::invalid_argument(
        "Snake head must be at a higher position than its tail.");
  }
}
