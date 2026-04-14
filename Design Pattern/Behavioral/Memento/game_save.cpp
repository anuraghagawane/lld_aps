#include <iostream>
#include <map>
using namespace std;

// Memento
class GameMemento {
  int health;
  int level;
  int position;

public:
  GameMemento(int health = 0, int level = 0, int position = 0)
      : health(health), level(level), position(position) {
    // TODO: Initialize fields
  }

  int getHealth() const {
    // TODO: Return health
    return health;
  }

  int getLevel() const {
    // TODO: Return level
    return level;
  }

  int getPosition() const {
    // TODO: Return position
    return position;
  }
};

// Originator
class Game {
private:
  int health = 100;
  int level = 1;
  int position = 0;

public:
  void play() {
    // TODO: Increment level by 1 and position by 10
    // TODO: Print "Playing... Level: X, Position: Y, Health: Z"
    level++;
    position += 10;
    cout << "Playing... Level: " << level << ", Position: " << position
         << ", Health: " << health << endl;
  }

  void takeDamage(int amount) {
    // TODO: Reduce health by amount
    // TODO: Print "Took X damage. Health: Y"
    health -= amount;
    cout << "Took " << amount << " damage. Health: " << health << endl;
  }

  GameMemento save() {
    // TODO: Return a new GameMemento with current state
    return GameMemento(health, level, position);
  }

  void restore(GameMemento memento) {
    // TODO: Restore health, level, and position from memento
    // TODO: Print "Game loaded: Level: X, Position: Y, Health: Z"
    health = memento.getHealth();
    level = memento.getLevel();
    position = memento.getPosition();
    cout << "Game loaded: Level: " << level << ", Position: " << position
         << ", Health: " << health << endl;
  }
};

// Caretaker
class SaveManager {
private:
  map<int, GameMemento> slots;

public:
  void save(Game &game, int slot) {
    // TODO: Save game state to the given slot
    slots[slot] = game.save();
  }

  void load(Game &game, int slot) {
    // TODO: Load game state from the given slot
    // TODO: Print "No save in slot X." if slot is empty
    if (slots.find(slot) != slots.end()) {
      game.restore(slots[slot]);
    } else {
      cout << "No save in slot" << slot << "." << endl;
    }
  }
};

int main() {
  Game game;
  SaveManager saveManager;

  game.play();               // Level 2, Position 10
  game.play();               // Level 3, Position 20
  saveManager.save(game, 0); // Save to slot 0

  game.takeDamage(50);       // Health: 50
  game.play();               // Level 4, Position 30
  saveManager.save(game, 1); // Save to slot 1

  game.takeDamage(40); // Health: 10
  cout << "\n--- Load Slot 0 ---" << endl;
  saveManager.load(game, 0); // Back to: Health 100, Level 3, Position 20

  cout << "\n--- Load Slot 1 ---" << endl;
  saveManager.load(game, 1); // Back to: Health 50, Level 4, Position 30

  return 0;
}
