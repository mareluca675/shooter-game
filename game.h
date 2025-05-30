#include <SFML/Graphics.hpp>
#include "player.h"

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
	void handlePlayerInput(sf::Keyboard::Key, bool);
	bool isMovingUp = false;
	bool isMovingDown = false;
	bool isMovingLeft = false;
	bool isMovingRight = false;

	// Time per frame
	const sf::Time TIME_PER_FRAME = sf::seconds(1.0f / 60.0f);
};

#endif