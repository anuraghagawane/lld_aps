#include "Scoreboard.hpp"
#include "../Game.hpp"
#include <iostream>
#include <mutex>

void Scoreboard::recordWin(const Player &player) {
  std::lock_guard<std::mutex> lock(mtx_);
  scores_[player.getName()]++;
}

int Scoreboard::getScore(const std::string &playerName) const {

  std::lock_guard<std::mutex> lock(mtx_);
  auto it = scores_.find(playerName);
  return it != scores_.end() ? it->second : 0;
}

void Scoreboard::printScoreboard() const {
  std::lock_guard<std::mutex> lock(mtx_);
  std::cout << "\n===== SCOREBOARD =====\n";
  if (scores_.empty()) {
    std::cout << "No games played yet.\n";
  } else {
    for (const auto &[name, score] : scores_) {
      std::cout << name << ": " << score << " wins\n";
    }
  }
  std::cout << "======================\n\n";
}

void Scoreboard::update(Game &game) { auto winner = game.getWinner(); }
