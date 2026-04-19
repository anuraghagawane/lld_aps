#include "Cell.hpp"

Cell::Cell() : symbol_(Symbol::EMPTY) {}
Symbol Cell::getSymbol() const { return symbol_; }
void Cell::setSymbol(Symbol symbol) { symbol_ = symbol; }
bool Cell::isEmpty() const { return symbol_ == Symbol::EMPTY; }
