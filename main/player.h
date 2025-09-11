#include <SFML/Graphics.hpp>

#ifndef PLAYER_H
#define PLAYER_H

class Player {
private:
	sf::Sprite sprite;
	sf::Vector2f aimDir;
	sf::Vector2f aimDirNorm;
	const float PLAYER_SPEED = 250.0f;
public:
	// Constructor
	Player();

	Player& operator=(const Player& other) {
		if (this != &other) {
			this->sprite = other.sprite;
			this->aimDir = other.aimDir;
			this->aimDirNorm = other.aimDirNorm;
		}

		return *this;
	}

	// Getters
	sf::Sprite& getSprite() { return sprite; }
	sf::Vector2f getCenter() const { return sf::Vector2f(sprite.getPosition().x, 
														 sprite.getPosition().y); }
	sf::Vector2f getAimDir() const { return aimDir; }
	sf::Vector2f getAimDirNorm() const { return aimDirNorm; }
	const float getPlayerSpeed() const { return PLAYER_SPEED; }

	// Setters
	void setAimDir(const sf::Vector2f& newAimDir) { aimDir = newAimDir; }
	void setAimDirNorm(const sf::Vector2f& newAimDirNorm) { aimDirNorm = newAimDirNorm; }
};

#endif
