#include "game.h"
#include <iostream>

Game::Game() {
	window.create(sf::VideoMode(1920, 1080), "Shooter");
	window.setFramerateLimit(60);
}

void Game::addBullet() {
	Bullet bullet;
	bullet.getShape().setPosition(player.getCenter());
	bullet.getShape().setFillColor(sf::Color::Red);
	bullet.setCurrVelocity(player.getAimDirNorm() * bullet.getSpeed());
	bullets.push_back(bullet);
}

void Game::handleMouseInput(sf::Mouse::Button button, bool isPressed = true) {
	if (button == sf::Mouse::Left && isPressed)
		addBullet();
	return;
}

void Game::handleKeyboardInput(sf::Keyboard::Key key, bool isPressed = true) {
	if (key == sf::Keyboard::W)
		isMovingUp = isPressed;
	if (key == sf::Keyboard::S)
		isMovingDown = isPressed;
	if (key == sf::Keyboard::A)
		isMovingLeft = isPressed;
	if (key == sf::Keyboard::D)
		isMovingRight = isPressed;
	return;
}

void Game::processEvents() {
	sf::Event event;
	while (window.pollEvent(event)) {
		switch (event.type) {
			case::sf::Event::KeyPressed:
				handleKeyboardInput(event.key.code);
				break;
			case::sf::Event::KeyReleased:
				handleKeyboardInput(event.key.code, false);
				break;
			case::sf::Event::MouseButtonPressed:
				handleMouseInput(event.mouseButton.button);
				break;
			case::sf::Event::MouseButtonReleased:
				handleMouseInput(event.mouseButton.button, false);
				break;
			case::sf::Event::Closed:
				window.close();
				break;
		}
	}
}

void Game::update(sf::Time deltaTime) {
	// Player movement
	
	// Handle input
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
	
	// Process player movement
	sf::Vector2f movement = direction * player.getPlayerSpeed();
	player.getShape().move(movement * deltaTime.asSeconds());

	// Player shooting logic

	// Set player center and aim direction
	player.setCenter(sf::Vector2f(player.getShape().getPosition().x + player.getShape().getRadius(), 
								  player.getShape().getPosition().y + player.getShape().getRadius()));
	player.setAimDir(sf::Vector2f(sf::Mouse::getPosition(window)) - player.getCenter());

	// N(V) = V / |V|
	player.setAimDirNorm(player.getAimDir() / sqrt(player.getAimDir().x * player.getAimDir().x
												 + player.getAimDir().y * player.getAimDir().y));
	// Process bullet movement
	for (Bullet& bullet : bullets) {
		bullet.getShape().move(bullet.getCurrVelocity() * deltaTime.asSeconds());
	}
}

void Game::render() {
	window.clear(sf::Color::Black);

	// Player rendering
	window.draw(player.getShape());

	// Bullets rendering
	for (Bullet& bullet : bullets) {
		window.draw(bullet.getShape());
	}

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