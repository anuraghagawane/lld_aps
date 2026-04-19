#ifndef GAME_HPP
#define GAME_HPP
#include "entities/Board.hpp"
#include "entities/Player.hpp"
#include "enums/enums.hpp"
#include "observers/GameObserver.hpp"
#include "strategies/WinningStrategy.hpp"
#include <array>
#include <memory>
#include <mutex>
#include <vector>
class Game {
private:
  std::unique_ptr<Board> board_;
  std::array<std::shared_ptr<Player>, 2> players_;
  int currentPlayerIndex_;
  GameStatus status_;
  std::vector<std::unique_ptr<WinningStrategy>> winningStrategies_;
  std::vector<GameObserver *> observers_;
  mutable std::mutex mtx_;

  void initializeStrategies();

  bool checkWin(int row, int col, Symbol symbol) const;

public:
  Game(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2,
       int boardSize);

  void makeMove(int row, int col);

  void addObserver(GameObserver *observer);
  void notifyObservers();
  const Board &getBoard() const;
  std::shared_ptr<Player> getCurrentPlayer() const;
  GameStatus getStatus() const;
  std::shared_ptr<Player> getWinner() const;
  void printBoard() const;
};
#endif // !GAME_HPP
