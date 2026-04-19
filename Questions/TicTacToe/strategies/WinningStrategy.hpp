#ifndef WINNING_STRATEGY_HPP
#define WINNING_STRATEGY_HPP

#include "../entities/Board.hpp"

class WinningStrategy {
public:
  virtual ~WinningStrategy() = default;
  virtual bool checkWin(const Board &board, int row, int col,
                        Symbol symbol) const = 0;
};

class RowWinningStrategy : public WinningStrategy {
  bool checkWin(const Board &board, int row, int col,
                Symbol symbol) const override;
};

class ColumnWinningStrategy : public WinningStrategy {
  bool checkWin(const Board &board, int row, int col,
                Symbol symbol) const override;
};

class DiagonalWinningStrategy : public WinningStrategy {
  bool checkWin(const Board &board, int row, int col,
                Symbol symbol) const override;
};
#endif // !WINNING_STRATEGY_HPP
