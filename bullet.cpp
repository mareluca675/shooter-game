#include "bullet.h"

Bullet::Bullet() {
	this->shape.setRadius(20.0f);
	this->shape.setFillColor(sf::Color::Red);
	currVelocity = sf::Vector2f(0.0f, 0.0f);
}