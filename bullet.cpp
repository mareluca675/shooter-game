#include "bullet.h"
#include <iostream>

Bullet::Bullet() {
	currVelocity = sf::Vector2f(0.0f, 0.0f);
	sprite.setScale(sf::Vector2f(0.1f, 0.1f));
}

bool Bullet::isOutOfBounds(sf::RenderWindow &window, Player &player) {
	if (this->initialPos.x < player.getSprite().getPosition().x - window.getSize().x / 2 ||
		this->initialPos.x > player.getSprite().getPosition().x + window.getSize().x / 2 ||
		this->initialPos.y < player.getSprite().getPosition().y - window.getSize().x / 2 ||
		this->initialPos.y > player.getSprite().getPosition().y + window.getSize().x / 2) {
		return true;
	}

	return false;
}