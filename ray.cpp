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
void Ray::calcHit(Player& player, sf::RenderWindow &window, GameMap& map) {
	// Calculate view edges
    float left_edge = player.getCenter().x - window.getSize().x / 2.f;
    float top_edge = player.getCenter().y - window.getSize().y / 2.f;
    float right_edge = left_edge + window.getSize().x;
    float bottom_edge = top_edge + window.getSize().y;

    // Calculate visible tile range, accounting for offsets
    int start_x = static_cast<int>(floor((left_edge - kOffsetX) / kTileWidthInPixels)) - 1;
    int end_x = static_cast<int>(ceil((right_edge - kOffsetX) / kTileWidthInPixels));
    int start_y = static_cast<int>(floor((top_edge - kOffsetY) / kTileHeightInPixels)) - 1;
    int end_y = static_cast<int>(ceil((bottom_edge - kOffsetY) / kTileHeightInPixels));

    // Draw tiles
    for (int i = start_x; i <= end_x; ++i) {
        for (int j = start_y; j <= end_y; ++j) {
            
        }
    }
}