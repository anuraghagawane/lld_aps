#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

enum class Color { WHITE, BLACK };

enum class MoveType { NORMAL, CAPTURE, CASTLING, PROMOTION, EN_PASSANT };

enum class GameStatus {
  PLAYING,
  PLAYER_1_WON,
  PLAYER_2_WON,
  DRAW,
  STALEMATE,
};

enum class PieceType { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

struct Position {
  int row;
  int col;
  Position(int row, int col) {
    this->row = row;
    this->col = col;
  }
};

string getColorTag(Color color) { return color == Color::BLACK ? "B" : "W"; }

class Player {
private:
  string name;
  Color color;

public:
  Player(const string &name, const Color color) : name(name), color(color) {}
  string getName() const { return name; }
  Color getColor() const { return color; }
};

class Piece;
class Board;

class Move {
private:
  Position from;
  Position to;

  shared_ptr<Piece> movedPiece;
  shared_ptr<Piece> capturedPiece;
  MoveType type;

  PieceType promotionPiece;

public:
  Move(Position from, Position to) : from(from), to(to) {}
  Position getFrom() const { return from; }
  Position getTo() const { return to; }

  shared_ptr<Piece> getMovedPiece() const { return movedPiece; }
  shared_ptr<Piece> getCapturedPiece() const { return capturedPiece; }

  MoveType getMoveType() const { return type; }

  PieceType getPromotedTo() const { return promotionPiece; }

  // setters;
  void setMovedPiece(shared_ptr<Piece> piece) { movedPiece = piece; }
  void setCapturePiece(shared_ptr<Piece> piece) { capturedPiece = piece; }
  void setMoveType(MoveType type) { this->type = type; }
  void setPromotionType(PieceType promotionPiece) {
    this->promotionPiece = promotionPiece;
  }
};

class Piece {
protected:
  Color color;
  bool hasMoved;

public:
  Piece(Color color) : color(color), hasMoved(false) {}
  virtual bool canMove(Board &board, const Move &move) = 0;
  virtual void display() = 0;
  Color getColor() const { return color; }
  bool getHasMoved() const { return hasMoved; }
  void setMoved() { hasMoved = true; }
};

class Cell {
private:
  shared_ptr<Piece> piece;
  Position position;

public:
  Cell(shared_ptr<Piece> piece, Position position)
      : piece(piece), position(position) {}
  shared_ptr<Piece> getPiece() { return piece; }
  Position getPosition() { return position; }

  void setPiece(shared_ptr<Piece> piece) { this->piece = piece; }
  void display() {
    if (!piece || piece == nullptr) {
      cout << "  ";
      return;
    }
    piece->display();
  }

  bool isEmpty() { return !piece || piece == nullptr; }
};

class Board {
private:
  vector<vector<Cell>> board =
      vector<vector<Cell>>(8, vector<Cell>(8, Cell(nullptr, Position(0, 0))));

public:
  Board();
  Cell &getCell(Position position) { return board[position.row][position.col]; }
  shared_ptr<Piece> getPiece(Position position) {
    return board[position.row][position.col].getPiece();
  }

  void display() {
    for (int i = 7; i >= 0; i--) {
      for (int j = 0; j < 8; j++) {
        cout << " | ";
        board[i][j].display();
      }
      cout << endl;
    }
  }

  void movePiece(Move &move) {
    auto from = move.getFrom();
    auto to = move.getTo();
    if (board[to.row][to.col].getPiece() != nullptr) {
      move.setCapturePiece(board[to.row][to.col].getPiece());
    }
    board[to.row][to.col].setPiece(board[from.row][from.col].getPiece());
    board[from.row][from.col].setPiece(nullptr);
    board[to.row][to.col].getPiece()->setMoved();
  }

  bool isPathClear(Position from, Position to) {
    if (from.row == to.row) {
      int start = min(from.col, to.col);
      int end = max(from.col, to.col);
      for (int c = start + 1; c < end; c++) {
        if (!board[from.row][c].isEmpty())
          return false;
      }

      return true;
    } else if (from.col == to.col) {
      int start = min(from.row, to.row);
      int end = max(from.row, to.row);
      for (int c = start + 1; c < end; c++) {
        if (!board[c][from.col].isEmpty())
          return false;
      }

      return true;
    } else if (abs(from.col - to.col) == abs(from.row - to.row)) {
      if (from.col - to.col == from.row - to.row) {
        Position start(0, 0);
        Position end(0, 0);
        if (from.col < to.col) {
          start = from;
          end = to;
        } else {
          start = to;
          end = from;
        }

        start.row++;
        start.col++;

        while (start.row < end.row && start.col < end.col) {
          if (!board[start.row][start.col].isEmpty()) {
            return false;
          }
          start.row++;
          start.col++;
        }

        return true;
      } else {
        Position start(0, 0);
        Position end(0, 0);
        if (from.col < to.col) {
          start = from;
          end = to;
        } else {
          start = to;
          end = from;
        }

        start.row--;
        start.col++;

        while (start.row > end.row && start.col < end.col) {
          if (!board[start.row][start.col].isEmpty()) {
            return false;
          }
          start.row--;
          start.col++;
        }

        return true;
      }
    }

    return false;
  }
};

class King : public Piece {
public:
  King(Color color) : Piece(color) {}

  bool canMove(Board &board, const Move &move) override {
    auto from = move.getFrom();
    auto to = move.getTo();

    if (abs(from.row - to.row) <= 1 && abs(from.col - to.col) <= 1) {
      return true;
    }

    return false;
  }

  void display() override { cout << "K" << getColorTag(color); }
};

class Queen : public Piece {
public:
  Queen(Color color) : Piece(color) {}

  bool canMove(Board &board, const Move &move) override {
    auto from = move.getFrom();
    auto to = move.getTo();

    // row move
    if (from.row == to.row)
      return board.isPathClear(from, to);
    // col move
    if (from.col == to.col)
      return board.isPathClear(from, to);

    return abs(from.row - to.row) == abs(from.col - to.col) &&
           board.isPathClear(from, to);
  }

  void display() override { cout << "Q" << getColorTag(color); }
};

class Bishop : public Piece {
public:
  Bishop(Color color) : Piece(color) {}
  bool canMove(Board &board, const Move &move) override {
    auto from = move.getFrom();
    auto to = move.getTo();

    return abs(from.row - to.row) == abs(from.col - to.col) &&
           board.isPathClear(from, to);
  }
  void display() override { cout << "B" << getColorTag(color); }
};

class Knight : public Piece {
public:
  Knight(Color color) : Piece(color) {}
  bool canMove(Board &board, const Move &move) override {
    vector<vector<int>> moves = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1},
                                 {1, 2}, {-1, 2}, {1, -2}, {-1, -2}};

    auto from = move.getFrom();
    auto to = move.getTo();
    for (auto &move : moves) {
      int newrow = from.row + move[0];
      int newcol = from.col + move[1];
      if (newrow < 0 || newrow > 7 || newcol < 0 || newcol > 7) {
        continue;
      }

      if (newrow == to.row && newcol == to.col) {
        return true;
      }
    }

    return false;
  }
  void display() override { cout << "H" << getColorTag(color); }
};

class Rook : public Piece {
public:
  Rook(Color color) : Piece(color) {}
  bool canMove(Board &board, const Move &move) override {
    auto from = move.getFrom();
    auto to = move.getTo();

    // row move
    if (from.row == to.row)
      return board.isPathClear(from, to);
    // col move
    if (from.col == to.col)
      return board.isPathClear(from, to);
    return false;
  }
  void display() override { cout << "R" << getColorTag(color); }
};

class Pawn : public Piece {
public:
  Pawn(Color color) : Piece(color) {}
  bool canMove(Board &board, const Move &move) override {
    auto from = move.getFrom();
    auto to = move.getTo();
    auto fromPiece = move.getMovedPiece();

    int offset = fromPiece->getHasMoved() ? 1 : 2;

    if (fromPiece->getColor() == Color::WHITE) {
      if (from.row >= to.row)
        return false;

      if (to.row == from.row + offset) {
        if (abs(to.col - from.col) == 1) {
          return board.getPiece(to) != nullptr;
        }
        return true;
      }
    } else {
      if (from.row <= to.row)
        return false;

      if (to.row == from.row - offset) {
        if (abs(to.col - from.col) == 1) {
          return board.getPiece(to) != nullptr;
        }
        return true;
      }
    }

    return false;
  }
  void display() override { cout << "P" << getColorTag(color); }
};

Board::Board() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      board[i][j] = Cell(nullptr, Position(i, j));
    }
  }

  board[0][0].setPiece(make_shared<Rook>(Color::WHITE));
  board[0][1].setPiece(make_shared<Knight>(Color::WHITE));
  board[0][2].setPiece(make_shared<Bishop>(Color::WHITE));
  board[0][3].setPiece(make_shared<Queen>(Color::WHITE));
  board[0][4].setPiece(make_shared<King>(Color::WHITE));
  board[0][5].setPiece(make_shared<Bishop>(Color::WHITE));
  board[0][6].setPiece(make_shared<Knight>(Color::WHITE));
  board[0][7].setPiece(make_shared<Rook>(Color::WHITE));

  board[1][0].setPiece(make_shared<Pawn>(Color::WHITE));
  board[1][1].setPiece(make_shared<Pawn>(Color::WHITE));
  board[1][2].setPiece(make_shared<Pawn>(Color::WHITE));
  board[1][3].setPiece(make_shared<Pawn>(Color::WHITE));
  board[1][4].setPiece(make_shared<Pawn>(Color::WHITE));
  board[1][5].setPiece(make_shared<Pawn>(Color::WHITE));
  board[1][6].setPiece(make_shared<Pawn>(Color::WHITE));
  board[1][7].setPiece(make_shared<Pawn>(Color::WHITE));

  board[6][0].setPiece(make_shared<Pawn>(Color::BLACK));
  board[6][1].setPiece(make_shared<Pawn>(Color::BLACK));
  board[6][2].setPiece(make_shared<Pawn>(Color::BLACK));
  board[6][3].setPiece(make_shared<Pawn>(Color::BLACK));
  board[6][4].setPiece(make_shared<Pawn>(Color::BLACK));
  board[6][5].setPiece(make_shared<Pawn>(Color::BLACK));
  board[6][6].setPiece(make_shared<Pawn>(Color::BLACK));
  board[6][7].setPiece(make_shared<Pawn>(Color::BLACK));

  board[7][0].setPiece(make_shared<Rook>(Color::BLACK));
  board[7][1].setPiece(make_shared<Knight>(Color::BLACK));
  board[7][2].setPiece(make_shared<Bishop>(Color::BLACK));
  board[7][3].setPiece(make_shared<Queen>(Color::BLACK));
  board[7][4].setPiece(make_shared<King>(Color::BLACK));
  board[7][5].setPiece(make_shared<Bishop>(Color::BLACK));
  board[7][6].setPiece(make_shared<Knight>(Color::BLACK));
  board[7][7].setPiece(make_shared<Rook>(Color::BLACK));
}

class Engine {
private:
  Board board;
  vector<Move> moves;

  shared_ptr<Player> player1;
  shared_ptr<Player> player2;

  GameStatus status;
  shared_ptr<Player> currentTurn;

public:
  Engine(shared_ptr<Player> player1, shared_ptr<Player> player2)
      : board(Board()), player1(player1), player2(player2),
        status(GameStatus::PLAYING), currentTurn(player1) {}

  void move(Position from, Position to) {
    auto fromCell = board.getCell(from);
    if (fromCell.getPiece() == nullptr) {
      cout << "Invalid move! play again!" << endl;
      return;
    }

    auto piece = fromCell.getPiece();
    auto topiece = board.getCell(to).getPiece();

    if (piece->getColor() != currentTurn->getColor()) {
      cout << "Wrong piece Moved" << endl;
      return;
    }

    if (topiece != nullptr && piece->getColor() == topiece->getColor()) {
      cout << "Cannot capture own piece. Play Again!" << endl;
      return;
    }

    auto move = Move(from, to);
    move.setMovedPiece(board.getPiece(from));
    if (!piece->canMove(board, move)) {
      cout << "Invalid move! play again!" << endl;
      return;
    }

    board.movePiece(move);

    moves.push_back(move);

    currentTurn = currentTurn == player1 ? player2 : player1;
  }

  void display() {
    cout << endl;
    board.display();
    cout << endl;
  }
};

int main() {
  cout << "Chess engine" << endl;
  auto player1 = make_shared<Player>("p1", Color::WHITE);
  auto player2 = make_shared<Player>("p2", Color::BLACK);
  auto engine = Engine(player1, player2);

  engine.display();
  engine.move(Position(1, 4), Position(3, 4));
  engine.display();
  engine.move(Position(0, 1), Position(2, 2));
  engine.display();
  engine.move(Position(6, 4), Position(3, 4));
  engine.display();
  engine.move(Position(2, 2), Position(3, 4));
  engine.display();
  return 0;
}
