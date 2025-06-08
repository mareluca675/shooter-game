#include "bullet.h"
#include <iostream>

Bullet::Bullet() {
	shape.setRadius(5.0f);
	currVelocity = sf::Vector2f(0.0f, 0.0f);
	sprite.setScale(sf::Vector2f(0.1f, 0.1f));
}

bool Bullet::isOutOfBounds(sf::RenderWindow &window, Player &player) {
	if (this->getShape().getPosition().x < player.getShape().getPosition().x - window.getSize().x / 2 ||
		this->getShape().getPosition().x > player.getShape().getPosition().x + window.getSize().x / 2 ||
		this->getShape().getPosition().y < player.getShape().getPosition().y - window.getSize().x / 2 ||
		this->getShape().getPosition().y > player.getShape().getPosition().y + window.getSize().x / 2) {
		std::cout << "Bullet out of bounds.\n";
		return true;
	}

	return false;
}