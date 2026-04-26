#include "Player.hpp"
Player::Player(const std::string &name) : name(name), position(0) {}

std::string Player::getName() const { return name; }

int Player::getPosition() const { return position; }

void Player::setPosition(int position) { this->position = position; }
