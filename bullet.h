#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>

class Bullet {
private:
	sf::CircleShape shape;
	sf::Vector2f currVelocity;
	const float SPEED = 2500.0f;
public:
	Bullet();
	
	// Getters
	sf::CircleShape& getShape() { return shape; }
	sf::Vector2f getCurrVelocity() const { return currVelocity; }
	const float getSpeed() const { return SPEED; }

	// Setters
	void setShape(const sf::CircleShape& newShape) { shape = newShape; }
	void setCurrVelocity(const sf::Vector2f& newVelocity) { currVelocity = newVelocity; }
};

#endif
