#include <SFML/Graphics.hpp>

#ifndef GAME_H
#define GAME_H

class Game {
public:
	Game();
	void run();
private:
	void processEvents();
	void update();
	void render();
private:
	sf::RenderWindow window;

	// Player
	sf::CircleShape player;
	void handlePlayerInput(sf::Keyboard::Key, bool);
	
	// Movement
	const float PLAYER_SPEED = 6.25f;
	bool isMovingUp = false;
	bool isMovingDown = false;
	bool isMovingLeft = false;
	bool isMovingRight = false;
};

#endif