#pragma once
class BoardEntity {
private:
  int start;
  int end;

public:
  BoardEntity(int start, int end);

  int getStart() const;
  int getEnd() const;

  virtual ~BoardEntity() = default;
};

class Ladder : public BoardEntity {
public:
  Ladder(int start, int end);
};

class Snake : public BoardEntity {
public:
  Snake(int start, int end);
};
