#pragma once
#include "entities/Board.hpp"
#include "entities/Dice.hpp"
#include "entities/Player.hpp"
#include "enum/GameStatus.hpp"
#include <queue>

class Game {
private:
  Board board;
  std::queue<Player> players;
  Dice dice;
  GameStatus status;
  Player *winner;

  Game(const Board &board, const std::queue<Player> &players, const Dice &dice);

public:
  void play();

private:
  void takeTurn(Player &player);

public:
  class Builder {
  private:
    Board *board;
    std::queue<Player> players;
    Dice *dice;

  public:
    Builder();
    Builder &setBoard(int boardSize,
                      const std::vector<BoardEntity *> &boardEntities);

    Builder &setPlayers(const std::vector<std::string> &playerNames);
    Builder &setDice(Dice *dice);
    Game build();
    ~Builder();
  };
};
