#include "game.h"

Game::Game() {
	window.create(sf::VideoMode(1920, 1080), "Shooter");
	window.setFramerateLimit(60);
	player.setRadius(50.f);
	player.setPosition(160.0f, 40.0f);
	player.setFillColor(sf::Color::Green);
}

void Game::run() {
	while (window.isOpen()) {
		processEvents();
		update();
		render();
	}
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
	if (key == sf::Keyboard::W)
		isMovingUp = isPressed;
	if (key == sf::Keyboard::S)
		isMovingDown = isPressed;
	if (key == sf::Keyboard::A)
		isMovingLeft = isPressed;
	if (key == sf::Keyboard::D)
		isMovingRight = isPressed;
}

void Game::processEvents() {
	sf::Event event;
	while (window.pollEvent(event)) {
		switch (event.type) {
			case::sf::Event::KeyPressed:
				handlePlayerInput(event.key.code, true);
				break;
			case::sf::Event::KeyReleased:
				handlePlayerInput(event.key.code, false);
				break;
			case::sf::Event::Closed:
				window.close();
				break;
		}
	}
}

void Game::update() {
	sf::Vector2f movement(0.0f, 0.0f);
	if (isMovingUp) movement.y -= PLAYER_SPEED;
	if (isMovingDown) movement.y += PLAYER_SPEED;
	if (isMovingLeft) movement.x -= PLAYER_SPEED;
	if (isMovingRight) movement.x += PLAYER_SPEED;
	player.move(movement);
}

void Game::render() {
	window.clear(sf::Color::Black);
	window.draw(player);
	window.display();
}