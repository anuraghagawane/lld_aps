#include "TicTacToeSystem.hpp"
#include <iostream>
#include <memory>
#include <stdexcept>
std::unique_ptr<TicTacToeSystem> TicTacToeSystem::instance_ = nullptr;
std::once_flag TicTacToeSystem::initFlag_;

TicTacToeSystem &TicTacToeSystem::getInstance() {
  call_once(initFlag_, []() { instance_.reset(new TicTacToeSystem()); });
  return *instance_;
}

Game &TicTacToeSystem::createGame(std::shared_ptr<Player> player1,
                                  std::shared_ptr<Player> player2) {
  currentGame_ = std::make_unique<Game>(player1, player2, 3);
  currentGame_->addObserver(scoreboard_.get());
  std::cout << "New game started: " << player1->getName() << " vs "
            << player2->getName() << std::endl;
  return *currentGame_;
}

void TicTacToeSystem::makeMove(const Player &player, int row, int col) {
  if (!currentGame_) {
    throw std::logic_error("No active game. Call createGame first.");
  }

  std::cout << player.getName() << " plays at (" << row << ", " << col << ")"
            << std::endl;
  currentGame_->makeMove(row, col);
  currentGame_->printBoard();
}

GameStatus TicTacToeSystem::getGameStatus() const {
  if (!currentGame_) {
    throw std::logic_error("No active game.");
  }

  return currentGame_->getStatus();
}

void TicTacToeSystem::printScoreboard() const {
  scoreboard_->printScoreboard();
}

void TicTacToeSystem::resetInstance() { instance_.reset(); }
