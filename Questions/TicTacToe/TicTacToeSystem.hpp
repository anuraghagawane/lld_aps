#ifndef TIC_TAC_TOE_SYSTEM_HPP
#define TIC_TAC_TOE_SYSTEM_HPP

#include "entities/Player.hpp"
#include "enums/enums.hpp"
#include "Game.hpp"
#include "observers/Scoreboard.hpp"
#include <memory>
#include <mutex>

class TicTacToeSystem {
private:
  static std::unique_ptr<TicTacToeSystem> instance_;
  static std::once_flag initFlag_;

  std::unique_ptr<Scoreboard> scoreboard_;
  std::unique_ptr<Game> currentGame_;

  TicTacToeSystem() : scoreboard_(std::make_unique<Scoreboard>()) {}

public:
  TicTacToeSystem(const TicTacToeSystem &) = delete;
  TicTacToeSystem &operator=(const TicTacToeSystem &) = delete;

  static TicTacToeSystem &getInstance();

  Game &createGame(std::shared_ptr<Player> player1,
                   std::shared_ptr<Player> player2);

  void makeMove(const Player &player, int row, int col);

  GameStatus getGameStatus() const;
  void printScoreboard() const;
  static void resetInstance();
};

#endif
