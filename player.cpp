#include "player.h"
#include <SFML/Graphics.hpp>

Player::Player() {
	shape.setSize(sf::Vector2f(200.0f, 100.0f));
	sprite.setScale(sf::Vector2f(0.5f, 0.5f));
}