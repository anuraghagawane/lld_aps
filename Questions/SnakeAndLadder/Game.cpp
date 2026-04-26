#include "Game.hpp"
#include "entities/Player.hpp"
#include <iostream>

Game::Game(const Board &board, const std::queue<Player> &players,
           const Dice &dice)
    : board(board), players(players), dice(dice), status(NOT_STARTED),
      winner(nullptr) {}

void Game::play() {
  if (players.size() < 2) {
    std::cout << "Cannot start game. At least 2 players are required."
              << std::endl;
    return;
  }

  this->status = RUNNING;

  while (status == RUNNING) {
    Player currentPlayer = players.front();
    players.pop();
    takeTurn(currentPlayer);

    if (status == RUNNING) {
      players.push(currentPlayer);
    }
  }

  std::cout << "Game Finised!" << std::endl;

  if (winner != nullptr) {
    std::cout << "The winner is " << winner->getName() << "!" << std::endl;
  }
}

void Game::takeTurn(Player &player) {
  int roll = dice.roll();
  std::cout << "\n"
            << player.getName() << "'s turn. Rolled a " << roll << "."
            << std::endl;

  int currentPosition = player.getPosition();
  int nextPosition = currentPosition + roll;

  if (nextPosition > board.getSize()) {
    std::cout << "Oops, " << player.getName() << " needs to land exactly on "
              << board.getSize() << ". Turn skipped." << std::endl;
    return;
  }

  if (nextPosition == board.getSize()) {
    player.setPosition(nextPosition);
    this->winner = &player;
    this->status = FINISHED;
    std::cout << "Hooray! " << player.getName() << " reached the final square "
              << board.getSize() << " and won!" << std::endl;
    return;
  }

  int finalPosition = board.getFinalPosition(nextPosition);
  if (finalPosition > nextPosition) {
    std::cout << "Wow! " << player.getName() << " found a ladder 🪜 at "
              << nextPosition << " and climbed to " << finalPosition << "."
              << std::endl;
  } else if (finalPosition < nextPosition) {
    std::cout << "Oh no! " << player.getName()
              << " was bitten by a snake 🐍 at " << nextPosition
              << " and slid down to " << finalPosition << "." << std::endl;
  } else {
    std::cout << player.getName() << " moved from " << currentPosition << " to "
              << finalPosition << "." << std::endl;
  }

  player.setPosition(finalPosition);

  if (roll == 6) {
    std::cout << player.getName() << " rolled a 6 and gets another turn!"
              << std::endl;
    takeTurn(player);
  }
}

Game::Builder::Builder() : board(nullptr), dice(nullptr) {}

Game::Builder::~Builder() {
  delete board;
  delete dice;
}

Game::Builder &
Game::Builder::setBoard(int boardSize,
                        const std::vector<BoardEntity *> &boardEntities) {
  this->board = new Board(boardSize, boardEntities);
  return *this;
}

Game::Builder &
Game::Builder::setPlayers(const std::vector<std::string> &playerNames) {
  std::queue<Player> tempPlayers;
  for (const std::string &playerName : playerNames) {
    tempPlayers.push(Player(playerName));
  }
  this->players = tempPlayers;
  return *this;
}

Game::Builder &Game::Builder::setDice(Dice *dice) {
  this->dice = dice;
  return *this;
}

Game Game::Builder::build() {
  if (board == nullptr || players.empty() || dice == nullptr) {
    throw std::invalid_argument("Board, Players, and Dice must be set.");
  }

  return Game(*board, players, *dice);
}
