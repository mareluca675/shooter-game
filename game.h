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
	sf::CircleShape player;	// Player movement
	const float PLAYER_SPEED = 6.25f;
	void handlePlayerInput(sf::Keyboard::Key, bool);
	bool isMovingUp;
	bool isMovingDown;
	bool isMovingLeft;
	bool isMovingRight;
};

#endif