#ifndef RAY_H
#define RAY_H

#include <SFML\Graphics.hpp>
#include "player.h"

class Ray {
private:
	sf::Vector2f relativeEnd;
public:
	Ray() = default;
	Ray(float x, float y);
	void calcHit(Player&, sf::Vector2f, sf::Vector2f);
	void reset(Player&);
	sf::Vector2f end;
};

#endif