#include "Game.hpp"
#include "enums/enums.hpp"
#include "exceptions/InvalidMoveException.hpp"
#include "observers/GameObserver.hpp"
#include <memory>
#include <mutex>
#include <string>

void Game::initializeStrategies() {
  winningStrategies_.push_back(std::make_unique<RowWinningStrategy>());
  winningStrategies_.push_back(std::make_unique<ColumnWinningStrategy>());
  winningStrategies_.push_back(std::make_unique<DiagonalWinningStrategy>());
}

bool Game::checkWin(int row, int col, Symbol symbol) const {
  for (const auto &strategy : winningStrategies_) {
    if (strategy->checkWin(*board_, row, col, symbol))
      return true;
  }

  return false;
}

Game::Game(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2,
           int boardSize)
    : board_(std::make_unique<Board>(boardSize)), players_({player1, player2}),
      currentPlayerIndex_(0), status_(GameStatus::IN_PROGRESS) {
  initializeStrategies();
}

void Game::makeMove(int row, int col) {
  std::lock_guard<std::mutex> lock(mtx_);

  if (status_ != GameStatus::IN_PROGRESS) {
    throw InvalidMoveException("Game is already over!");
  }

  if (!board_->isCellEmpty(row, col)) {
    throw InvalidMoveException("Cell (" + std::to_string(row) + ", " +
                               std::to_string(col) + ") is already occupied");
  }

  auto &currentPlayer = players_[currentPlayerIndex_];
  board_->placeSymbol(row, col, currentPlayer->getSymbol());

  if (checkWin(row, col, currentPlayer->getSymbol())) {
    status_ = (currentPlayer->getSymbol() == Symbol::X) ? GameStatus::WINNER_X
                                                        : GameStatus::WINNER_O;
    notifyObservers();
    return;
  }

  if (board_->isFull()) {
    status_ = GameStatus::DRAW;
    notifyObservers();
    return;
  }

  currentPlayerIndex_ = (currentPlayerIndex_ + 1) % 2;
}

void Game::addObserver(GameObserver *observer) {
  std::lock_guard<std::mutex> lock(mtx_);
  observers_.push_back(observer);
}

void Game::notifyObservers() {
  for (auto &observer : observers_) {
    observer->update(*this);
  }
}

const Board &Game::getBoard() const { return *board_; }

std::shared_ptr<Player> Game::getCurrentPlayer() const {
  return players_[currentPlayerIndex_];
}

GameStatus Game::getStatus() const { return status_; }

std::shared_ptr<Player> Game::getWinner() const {
  if (status_ == GameStatus::WINNER_X) {
    return players_[0]->getSymbol() == Symbol::X ? players_[0] : players_[1];
  } else if (status_ == GameStatus::WINNER_O) {
    return players_[0]->getSymbol() == Symbol::O ? players_[0] : players_[1];
  }

  return nullptr;
}

void Game::printBoard() const { board_->printBoard(); }
