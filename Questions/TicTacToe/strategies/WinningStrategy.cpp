#include "WinningStrategy.hpp"

bool ColumnWinningStrategy::checkWin(const Board &board, int row, int col,
                                     Symbol symbol) const {
  int size = board.getSize();
  for (int r = 0; r < size; r++) {
    if (board.getCell(r, col).getSymbol() != symbol)
      return false;
  }

  return true;
}

bool RowWinningStrategy::checkWin(const Board &board, int row, int col,
                                  Symbol symbol) const {
  int size = board.getSize();
  for (int c = 0; c < size; c++) {
    if (board.getCell(row, c).getSymbol() != symbol) {
      return false;
    }
  }
  return true;
}

bool DiagonalWinningStrategy::checkWin(const Board &board, int row, int col,
                                       Symbol symbol) const {
  int size = board.getSize();

  // Check main diagonal (top-left to bottom-right)
  bool mainDiagonalWin = true;
  for (int i = 0; i < size; i++) {
    if (board.getCell(i, i).getSymbol() != symbol) {
      mainDiagonalWin = false;
      break;
    }
  }
  if (mainDiagonalWin)
    return true;

  // Check anti-diagonal (top-right to bottom-left)
  for (int i = 0; i < size; i++) {
    if (board.getCell(i, size - 1 - i).getSymbol() != symbol) {
      return false;
    }
  }
  return true;
}
