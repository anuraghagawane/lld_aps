#include "Game.hpp"
#include "entities/BoardEntity.hpp"
#include "entities/Dice.hpp"
#include <vector>

using namespace std;

class SnakeAndLadderDemo {
public:
  static int main() {
    vector<BoardEntity *> boardEntities = {
        new Snake(17, 7),   new Snake(54, 34), new Snake(62, 19),
        new Snake(98, 79),  new Ladder(3, 38), new Ladder(24, 33),
        new Ladder(42, 93), new Ladder(72, 84)

    };

    vector<string> players = {"Alice", "Bob", "Charlie"};

    Game::Builder builder;
    Game game = builder.setBoard(100, boardEntities)
                    .setPlayers(players)
                    .setDice(new Dice(1, 6))
                    .build();

    game.play();

    for (BoardEntity *entity : boardEntities) {
      delete entity;
    }
    return 0;
  }
};

int main() { return SnakeAndLadderDemo::main(); }
