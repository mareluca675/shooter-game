#include "game.h"

Game::Game() {
	window.create(sf::VideoMode(1920, 1080), "Shooter");
	window.setFramerateLimit(60);
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

void Game::update(sf::Time deltaTime) {
	sf::Vector2f direction(0.0f, 0.0f);
	if (isMovingUp) direction.y -= 1;
	if (isMovingDown) direction.y += 1;
	if (isMovingLeft) direction.x -= 1;
	if (isMovingRight) direction.x += 1;

	// Calculate magnitude
	float magnitude = sqrt(direction.x * direction.x + direction.y * direction.y);
	if (magnitude) {
		direction /= magnitude;
	}

	sf::Vector2f movement = direction * player.getPlayerSpeed();
	player.getShape().move(movement * deltaTime.asSeconds());
}

void Game::render() {
	window.clear(sf::Color::Black);
	window.draw(player.getShape());
	window.display();
}

void Game::run() {
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (window.isOpen()) {
		processEvents();
		timeSinceLastUpdate += clock.restart();

		while (timeSinceLastUpdate > TIME_PER_FRAME) {
			timeSinceLastUpdate -= TIME_PER_FRAME;
			processEvents();
			update(TIME_PER_FRAME);
		}

		render();
	}
}