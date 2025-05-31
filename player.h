#include <SFML/Graphics.hpp>

#ifndef PLAYER_H
#define PLAYER_H

class Player {
private:
	sf::CircleShape shape;
	sf::Vector2f center;
	sf::Vector2f aimDir;
	sf::Vector2f aimDirNorm;
	const float PLAYER_SPEED = 200.0f;
public:
	// Constructor
	Player();

	// Getters
	sf::CircleShape& getShape() { return shape; }
	sf::Vector2f getCenter() { return center; }
	sf::Vector2f getAimDir() { return aimDir; }
	sf::Vector2f getAimDirNorm() { return aimDirNorm; }
	const float getPlayerSpeed() const { return PLAYER_SPEED; }

	// Setters
	void setCenter(const sf::Vector2f& newCenter) { center = newCenter; }
	void setAimDir(const sf::Vector2f& newAimDir) { aimDir = newAimDir; }
	void setAimDirNorm(const sf::Vector2f& newAimDirNorm) { aimDirNorm = newAimDirNorm; }
};

#endif
