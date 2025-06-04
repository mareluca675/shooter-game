#include "ray.h"

Ray::Ray(float x, float y) {
	// Set end point relative to mouse cursor
	// Add arbitrary length
	relativeEnd = sf::Vector2f(x, y) * 3000.f;
}

// Reset end-point of ray
void Ray::reset(Player& player) {
	// Set end-point to (default length) distance away from mouse in set direction
	end = player.getCenter() + relativeEnd;
}

// Calculates intersection-point two lines
// Used for getting intersection between ray and wall
// More or less black-box code
void Ray::calcHit(Player& player, sf::Vector2f p3, sf::Vector2f p4) {
	const sf::Vector2f p1 = player.getCenter();
	const sf::Vector2f p2 = end;

	// Calculates denominator of equations
	const float den = (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);

	// Lines are paralel
	if (den == 0) {
		return;
	}

	const float t = ((p1.x - p3.x) * (p3.y - p4.y) - (p1.y - p3.y) * (p3.x - p4.x)) / den;
	const float u = -((p1.x - p2.x) * (p1.y - p3.y) - (p1.y - p2.y) * (p1.x - p3.x)) / den;

	// If there's an intersection...
	if (t > 0 && t < 1 && u > 0 && u < 1) {
		// Gets intersection point
		end.x = p1.x + t * (p2.x - p1.x);
		end.y = p1.y + t * (p2.y - p1.y);
	}
}