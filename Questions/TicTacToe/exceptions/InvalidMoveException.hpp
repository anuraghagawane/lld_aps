#ifndef INVALID_MOVE_EXCEPTION_HPP
#define INVALID_MOVE_EXCEPTION_HPP

#include <stdexcept>
class InvalidMoveException : public std::runtime_error {
public:
  explicit InvalidMoveException(const std::string &message)
      : std::runtime_error(message) {}
};

#endif // !INVALID_MOVE_EXCEPTION_HPP
