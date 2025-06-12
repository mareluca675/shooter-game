#include <SFML/Graphics.hpp>
#include <Candle/Candle.hpp>
#include "resource_identifiers.h"
#include "player.h"
#include "bullet.h"
#include "game_map.h"
#include "config.h"
#include "rng.h"


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

	// Texture holder
	ResourceHolder<sf::Texture, Textures::ID> textureHolder;

	// Window
	sf::RenderWindow window;

	// Light source
	candle::RadialLight playerLight;
	candle::EdgeVector edges;

	// Camera
	sf::View cameraView;
	
	// Game map
	GameMap* gameMap;

	// Player 
	Player player;
	void spawnPlayer();
	void handleKeyboardInput(sf::Keyboard::Key, bool);
	void handleMouseInput(sf::Mouse::Button, bool);
	bool isMovingUp = false;
	bool isMovingDown = false;
	bool isMovingLeft = false;
	bool isMovingRight = false;
	float mouseAngle = 0.0f;

	// Bullets
	std::vector<std::pair<Bullet, candle::RadialLight>> bullets;
	void addBullet();

	// Time per frame
	const sf::Time TIME_PER_FRAME = sf::seconds(1.0f / 60.0f);
};

#endif