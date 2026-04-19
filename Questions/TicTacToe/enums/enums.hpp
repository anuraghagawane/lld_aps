#ifndef ENUMS_HPP
#define ENUMS_HPP
enum class GameStatus { IN_PROGRESS, WINNER_X, WINNER_O, DRAW };
enum class Symbol { X, O, EMPTY };
enum class Symbol;

char getDisplayChar(Symbol symbol);
#endif // !ENUMS_HPP>
