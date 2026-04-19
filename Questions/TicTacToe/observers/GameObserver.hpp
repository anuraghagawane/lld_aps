#ifndef GAME_OBSERVER_HPP
#define GAME_OBSERVER_HPP

class Game;
class GameObserver {
public:
  virtual ~GameObserver() = default;
  virtual void update(Game &game) = 0;
};

#endif // !GAME_OBSERVER_HPP
