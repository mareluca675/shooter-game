#include "player.h"
#include <SFML/Graphics.hpp>

Player::Player() {
	shape.setRadius(50.f);
	shape.setPosition(160.0f, 40.0f);
	shape.setFillColor(sf::Color::Green);
}