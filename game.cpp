#include "game.h"
#include <iostream>

Game::Game() {
    window.create(sf::VideoMode(kScreenWidth, kScreenHeight), "Shooter");  
    window.setFramerateLimit(60);  
    gameMap = new GameMap(kMapWidthInTiles, kMapHeightInTiles,  
                          kMapFillPercentage, kTileWidthInPixels,  
                          kTileHeightInPixels, window);  
	cameraView.reset(sf::FloatRect(0, 0, static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
	textureHolder.load(Textures::Bullet, "assets/textures/bullet.png");
	textureHolder.load(Textures::Player, "assets/textures/player.png");
}

void Game::addBullet() {
	Bullet bullet;
	bullet.getShape().setPosition(player.getCenter());
	bullet.getSprite().setPosition(player.getCenter());
	bullet.getSprite().setTexture(textureHolder.get(Textures::Bullet));
	bullet.setCurrVelocity(player.getAimDirNorm() * bullet.getSpeed());
	bullet.getSprite().setRotation(mouseAngle + 90.0f);
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
		gameMap->ProcessMap(window), spawnPlayer();
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

	// Rotation
	sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    mouseAngle = (-atan2(mousePosition.x - player.getSprite().getPosition().x,
					     mousePosition.y - player.getSprite().getPosition().y)) * 180.0f / PI;

	Player tempPlayerRotation = player;
	tempPlayerRotation.getShape().setRotation(mouseAngle + 90.0f);

	if (!gameMap->isCollidingPlayer(tempPlayerRotation)) {
		player.getShape().setRotation(mouseAngle + 90.0f);
		player.getSprite().setRotation(mouseAngle + 90.0f);
	}

	sf::Vector2f movement = direction * player.getPlayerSpeed() * deltaTime.asSeconds();

	// Check x movement
	Player tempPlayerX = player;
	tempPlayerX.getSprite().move(movement.x, 0);
	tempPlayerX.getShape().move(movement.x, 0);
	if (!gameMap->isCollidingPlayer(tempPlayerX)) {
		player.getSprite().move(movement.x, 0);
		player.getShape().move(movement.x, 0);
	}

	// Check y movement
	Player tempPlayerY = player;
	tempPlayerY.getSprite().move(0, movement.y);
	tempPlayerY.getShape().move(0, movement.y);
	if (!gameMap->isCollidingPlayer(tempPlayerY)) {
		player.getSprite().move(0, movement.y);
		player.getShape().move(0, movement.y);
	}
	
	//std::cout << "Player at position: " << player.getShape().getPosition().x << ' ' << player.getShape().getPosition().y << '\n';

	// Player shooting logic

	// Update camera view to follow the player	
	cameraView.setCenter(player.getCenter());

	// Set player center and aim direction
	sf::Vector2f mouseWorldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window), cameraView);
	player.setAimDir(mouseWorldPos - player.getCenter());

	// N(V) = V / |V|
	player.setAimDirNorm(player.getAimDir() / sqrt(player.getAimDir().x * player.getAimDir().x
					   + player.getAimDir().y * player.getAimDir().y));

	/*std::cout << "Aim dir normalized: " << player.getAimDirNorm().x << ", "
										<< player.getAimDirNorm().y << std::endl;*/

	// Process bullet movement
	for (auto it = bullets.begin(); it != bullets.end();) {
		if (it->isOutOfBounds(window, player) || gameMap->isCollidingBullet(*it)) {
			it = bullets.erase(it);
		}
		else {
			it->getShape().move(it->getCurrVelocity() * deltaTime.asSeconds());
			it->getSprite().move(it->getCurrVelocity() * deltaTime.asSeconds());
			++it;
		}
	}
}

void Game::render() {
	window.clear(sf::Color::Black);

	// Adjusting camera view
	window.setView(cameraView);

	// Game map rendering
	gameMap->DrawMap(window, player, kOffsetX, kOffsetY);

	// Player rendering
	window.draw(player.getSprite());

	// Bullets rendering
	for (Bullet& bullet : bullets) {
		/*bullet.getShape().setFillColor(sf::Color::Red);
		window.draw(bullet.getShape());*/
		window.draw(bullet.getSprite());
	}

	window.display();
}

void Game::spawnPlayer() {
	player.getShape().setPosition(player.getCenter());
	player.getSprite().setPosition(sf::Vector2f(0.0f, 0.0f));
	player.getSprite().setTexture(textureHolder.get(Textures::Player));

	while (gameMap->isCollidingPlayer(player)) {
		sf::Vector2f position(Rng::IntInRange(0, 10000), Rng::IntInRange(0, 10000));
		player.getShape().setPosition(position);
		player.getSprite().setPosition(position);
		std::cout << "Respawned at " << position.x << ' ' << position.y << '\n';
	}
}

void Game::run() {
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	gameMap->ProcessMap(window);
	spawnPlayer();

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