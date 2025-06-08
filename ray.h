#ifndef RAY_H
#define RAY_H

#include <SFML\Graphics.hpp>
#include "player.h"
#include "config.h"
#include "game_map.h"

class Ray {
private:
	sf::Vector2f relativeEnd;
public:
	Ray() = default;
	Ray(float x, float y);
	void calcHit(Player&, sf::RenderWindow&, GameMap&);
	void reset(Player&);
	sf::Vector2f end;
};

#endif