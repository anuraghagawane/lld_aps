#include "TicTacToeSystem.hpp"
#include "enums/enums.hpp"
#include <iostream>
#include <memory>

using namespace std;
int main() {
  auto &system = TicTacToeSystem::getInstance();
  auto alice = make_shared<Player>("Alice", Symbol::X);
  auto bob = make_shared<Player>("Bob", Symbol::O);

  // Game 1: Alice wins
  cout << "========== GAME 1 ==========\n";
  system.createGame(alice, bob);

  system.makeMove(*alice, 0, 0); // X at (0,0)
  system.makeMove(*bob, 1, 0);   // O at (1,0)
  system.makeMove(*alice, 0, 1); // X at (0,1)
  system.makeMove(*bob, 1, 1);   // O at (1,1)
  system.makeMove(*alice, 0, 2); // X at (0,2) - Alice wins!

  cout << "Game 1 Result: ";
  switch (system.getGameStatus()) {
  case GameStatus::WINNER_X:
    cout << "WINNER_X\n";
    break;
  case GameStatus::WINNER_O:
    cout << "WINNER_O\n";
    break;
  case GameStatus::DRAW:
    cout << "DRAW\n";
    break;
  default:
    cout << "IN_PROGRESS\n";
  }

  // Game 2: Bob wins
  cout << "\n========== GAME 2 ==========\n";
  system.createGame(alice, bob);

  system.makeMove(*alice, 0, 0); // X at (0,0)
  system.makeMove(*bob, 1, 1);   // O at (1,1) - center
  system.makeMove(*alice, 0, 1); // X at (0,1)
  system.makeMove(*bob, 0, 2);   // O at (0,2)
  system.makeMove(*alice, 2, 0); // X at (2,0)
  system.makeMove(*bob, 2, 2);   // O at (2,2) - Bob wins diagonal!

  cout << "Game 2 Result: ";
  switch (system.getGameStatus()) {
  case GameStatus::WINNER_X:
    cout << "WINNER_X\n";
    break;
  case GameStatus::WINNER_O:
    cout << "WINNER_O\n";
    break;
  case GameStatus::DRAW:
    cout << "DRAW\n";
    break;
  default:
    cout << "IN_PROGRESS\n";
  }

  // Game 3: Draw
  cout << "\n========== GAME 3 ==========\n";
  system.createGame(alice, bob);

  system.makeMove(*alice, 0, 0); // X
  system.makeMove(*bob, 0, 1);   // O
  system.makeMove(*alice, 0, 2); // X
  system.makeMove(*bob, 1, 1);   // O
  system.makeMove(*alice, 1, 0); // X
  system.makeMove(*bob, 1, 2);   // O
  system.makeMove(*alice, 2, 1); // X
  system.makeMove(*bob, 2, 0);   // O
  system.makeMove(*alice, 2, 2); // X - Draw!

  cout << "Game 3 Result: ";
  switch (system.getGameStatus()) {
  case GameStatus::WINNER_X:
    cout << "WINNER_X\n";
    break;
  case GameStatus::WINNER_O:
    cout << "WINNER_O\n";
    break;
  case GameStatus::DRAW:
    cout << "DRAW\n";
    break;
  default:
    cout << "IN_PROGRESS\n";
  }

  // Final scoreboard
  system.printScoreboard();

  return 0;
}
