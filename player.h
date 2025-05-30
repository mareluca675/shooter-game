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
};

#endif
