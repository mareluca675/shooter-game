#include <SFML/Graphics.hpp>
#include "player.h"
#include "bullet.h"

#ifndef GAME_H
#define GAME_H

class Game {
public:
	Game();
	void run();
private:
	void processEvents();
	void update(sf::Time);
	void render();
private:
	sf::RenderWindow window;
	
	// Player 
	Player player;
	void handleKeyboardInput(sf::Keyboard::Key, bool);
	void handleMouseInput(sf::Mouse::Button, bool);
	bool isMovingUp = false;
	bool isMovingDown = false;
	bool isMovingLeft = false;
	bool isMovingRight = false;

	// Bullets
	std::vector<Bullet> bullets;
	void addBullet();

	// Time per frame
	const sf::Time TIME_PER_FRAME = sf::seconds(1.0f / 60.0f);
};

#endif