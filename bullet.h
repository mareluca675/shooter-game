#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include "player.h"
#include "config.h"

class Bullet {
private:
	sf::CircleShape shape;
	sf::Vector2f currVelocity;
	const float SPEED = 2500.0f;
public:
	Bullet();
	bool isOutOfBounds(sf::RenderWindow&, Player&);

	Bullet& operator=(const Bullet& other) {
		if (this != &other) { // Prevent self-assignment issues
			shape = other.shape;
			currVelocity = other.currVelocity;
		}

		return *this;
	}

	// Getters
	sf::CircleShape& getShape() { return shape; }
	sf::Vector2f getCurrVelocity() const { return currVelocity; }
	sf::Vector2f getCenter() const { return sf::Vector2f(shape.getPosition().x + shape.getRadius(), shape.getPosition().y + shape.getRadius()); }
	const float getSpeed() const { return SPEED; }

	// Setters
	void setShape(const sf::CircleShape& newShape) { shape = newShape; }
	void setCurrVelocity(const sf::Vector2f& newVelocity) { currVelocity = newVelocity; }
};

#endif
