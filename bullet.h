#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include "player.h"
#include "config.h"

class Bullet {
private:
	sf::Sprite sprite;
	sf::Vector2f currVelocity;
	const float SPEED = 2500.0f;
public:
	Bullet();
	bool isOutOfBounds(sf::RenderWindow&, Player&);
	sf::Vector2f initialPos;

	Bullet& operator=(const Bullet& other) {
		if (this != &other) { // Prevent self-assignment issues
			currVelocity = other.currVelocity;
			sprite = other.sprite;
		}

		return *this;
	}

	// Getters
	sf::Sprite& getSprite() { return sprite; }
	sf::Vector2f getCurrVelocity() const { return currVelocity; }
	sf::Vector2f getCenter() const { return sf::Vector2f(sprite.getTexture()->getSize().x / 2.0f + sprite.getPosition().x,
														 sprite.getTexture()->getSize().y / 2.0f + sprite.getPosition().y); }
	const float getSpeed() const { return SPEED; }
	
	// Setters
	void setCurrVelocity(const sf::Vector2f& newVelocity) { currVelocity = newVelocity; }
};

#endif
