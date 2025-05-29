#include <SFML/Graphics.hpp>

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
	sf::CircleShape player;	
	
	// Player movement
	const float PLAYER_SPEED = 200.0f;
	void handlePlayerInput(sf::Keyboard::Key, bool);
	bool isMovingUp = false;
	bool isMovingDown = false;
	bool isMovingLeft = false;
	bool isMovingRight = false;

	// Time per frame
	const sf::Time TIME_PER_FRAME = sf::seconds(1.0f / 60.0f);
};

#endif