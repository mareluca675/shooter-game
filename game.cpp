#include "game.h"
#include "rng.h"
#include <iostream>

Game::Game() {
    window.create(sf::VideoMode(kScreenWidth, kScreenHeight), "Shooter");  
    window.setFramerateLimit(60);  
    gameMap = new GameMap(kMapWidthInTiles, kMapHeightInTiles,  
                          kMapFillPercentage, kTileWidthInPixels,  
                          kTileHeightInPixels, window);  
	cameraView.reset(sf::FloatRect(0, 0, static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
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
	if (key == sf::Keyboard::G)
		gameMap->ProcessMap(window);
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

	sf::Vector2f movement = direction * player.getPlayerSpeed() * deltaTime.asSeconds();

	// Check x movement
	Player tempPlayerX = player;
	tempPlayerX.getShape().move(movement.x, 0);
	if (!gameMap->isColliding(tempPlayerX)) {
		player.getShape().move(movement.x, 0);
	}

	// Check y movement
	Player tempPlayerY = player; // Use updated position after x movement
	tempPlayerY.getShape().move(0, movement.y);
	if (!gameMap->isColliding(tempPlayerY)) {
		player.getShape().move(0, movement.y);
	}

	std::cout << player.getShape().getPosition().x << ' ' << player.getShape().getPosition().y << '\n';

	// Player shooting logic

	// Update camera view to follow the player	
	cameraView.setCenter(player.getCenter());

	// Set player center and aim direction
	player.setCenter(sf::Vector2f(player.getShape().getPosition().x + player.getShape().getRadius(),
		player.getShape().getPosition().y + player.getShape().getRadius()));
	sf::Vector2f mouseWorldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window), cameraView);
	player.setAimDir(mouseWorldPos - player.getCenter());

	// N(V) = V / |V|
	player.setAimDirNorm(player.getAimDir() / sqrt(player.getAimDir().x * player.getAimDir().x
						+ player.getAimDir().y * player.getAimDir().y));

	/*std::cout << "Aim dir normalized: " << player.getAimDirNorm().x << ", "
										<< player.getAimDirNorm().y << std::endl;*/

	// Process bullet movement
	for (Bullet& bullet : bullets) {
		bullet.getShape().move(bullet.getCurrVelocity() * deltaTime.asSeconds());
	}
}

void Game::render() {
	window.clear(sf::Color::Black);

	// Adjusting camera view
	window.setView(cameraView);

	// Game map rendering
	gameMap->DrawMap(window, kOffsetX, kOffsetY);

	// Player rendering
	window.draw(player.getShape());

	// Bullets rendering
	for (Bullet& bullet : bullets)
		window.draw(bullet.getShape());

	window.display();
}

void Game::run() {
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	gameMap->ProcessMap(window);

	player.setCenter(sf::Vector2f(0.0f, 0.0f));
	player.getShape().setPosition(sf::Vector2f(0.0f, 0.0f));

	int attempts = 0;
	const int maxAttempts = 100; // Prevent infinite loop

	std::cout << gameMap->isColliding(player) << '\n';

	while (gameMap->isColliding(player) && attempts < maxAttempts) {
		sf::Vector2f position;
		position.x = Rng::IntInRange(0, 10000);
		position.y = Rng::IntInRange(0, 10000);
		player.getShape().setPosition(position);
		player.setCenter(position);
		std::cout << "Respawned at " << position.x << ' ' << position.y << '\n';
		attempts++;
	}

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