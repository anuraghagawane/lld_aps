#ifndef SCORE_BOARD_HPP
#define SCORE_BOARD_HPP

#include "../entities/Player.hpp"
#include "GameObserver.hpp"
#include <mutex>
#include <string>
#include <unordered_map>

class Scoreboard : public GameObserver {
private:
  std::unordered_map<std::string, int> scores_;
  mutable std::mutex mtx_;

public:
  Scoreboard() = default;
  void update(Game &game) override;
  void recordWin(const Player &player);
  int getScore(const std::string &playerName) const;
  void printScoreboard() const;
};

#endif // !SCROE_BOARD_HPP
