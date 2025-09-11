#include <vector>
#include <iostream>

#include "game_map.h"
#include "rng.h"
#include "room.h"
#include "util.h"
#include "config.h"

GameMap::GameMap(int width, int height, int map_fill_percentage,
    int tile_width_in_pixels, int tile_height_in_pixels,
    sf::RenderWindow& window) :
    width_(width), height_(height),
    map_fill_percentage_(map_fill_percentage),
    tile_width_in_pixels_(tile_width_in_pixels),
    tile_height_in_pixels_(tile_height_in_pixels) {
    char_map_ = std::vector<std::vector<char> >(width, std::vector<char>(height, '1'));
    // PrintMapToConsole();
    RandomizeMap(0);
}

void GameMap::ProcessMap(sf::RenderWindow& window) {
    RandomizeMap(map_fill_percentage_);
    SmoothMap(kNumIterationsSmooth);
    room_vector_ = GenerateRooms();
    ConnectRooms(room_vector_);
    // DrawRoomConnections(window);
    SmoothMap(kNumIterationsSmooth);
    CreatePassageways();
    RemoveConnectedComponents(room_vector_);
}

void GameMap::Reset() {
    char_map_ = std::vector<std::vector<char> >(Width(), std::vector<char>(Height(), '1'));
}

void GameMap::PrintMapToConsole() {
    std::cout << std::endl;
    for (int i = 0; i < char_map_.size(); ++i) {
        std::cout << " ";
        for (int j = 0; j < char_map_[0].size(); ++j) {
            std::cout << char_map_[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void GameMap::RandomizeMap(int map_fill_percent) {
    for (int i = 0; i < char_map_.size(); ++i) {
        for (int j = 0; j < char_map_[0].size(); ++j) {
            if ((i == 0) || (j == 0) || (i == char_map_.size() - 1) || (j == char_map_[0].size() - 1)) {
                char_map_[i][j] = '1';
                continue;
            }

            int rand_int = Rng::IntInRange(0, 100);
            if (rand_int < map_fill_percent) {
                char_map_[i][j] = '1';
            }
            else {
                char_map_[i][j] = '0';
            }
        }
    }
}

bool GameMap::IsValidCell(int i, int j) {
    return (i >= 0) && (i < Height()) && (j >= 0) && (j < Width());
}

int GameMap::CountNeighborWalls(int i, int j) {
    int cnt = 0;
    for (int dx = -1; dx < 2; ++dx) {
        for (int dy = -1; dy < 2; ++dy) {
            if ((dx == 0) && (dy == 0)) {
                continue;
            }
            if (IsValidCell(i + dy, j + dx) && (char_map_[i + dy][j + dx] == '1')) {
                ++cnt;
            }
        }
    }
    return cnt;
}

void GameMap::SmoothMap(int num_iterations) {
    for (int k = 0; k < num_iterations; ++k) {
        for (int i = 1; i < char_map_.size() - 1; ++i) {
            for (int j = 1; j < char_map_[0].size() - 1; ++j) {
                char curr = char_map_[i][j];
                int cnt = CountNeighborWalls(i, j);

                if (curr == '0') { // If the cell is dead
                    if (std::find(BIRTH_VALUES.begin(), BIRTH_VALUES.end(), cnt) !=
                        BIRTH_VALUES.end()) {
                        char_map_[i][j] = '1';
                    }
                }
                else { // If the cell is alive
                    if (std::find(SURVIVE_VALUES.begin(), SURVIVE_VALUES.end(), cnt) ==
                        SURVIVE_VALUES.end()) {
                        char_map_[i][j] = '0';
                    }
                }
            }
        }
    }
}

//void GameMap::DrawMap(sf::RenderWindow& window, Player& player, double offset_x, double offset_y, candle::EdgeVector& edges) {
//    // Calculate view edges
//    float left_edge = player.getCenter().x - window.getSize().x / 2.f;
//    float top_edge = player.getCenter().y - window.getSize().y / 2.f;
//    float right_edge = left_edge + window.getSize().x;
//    float bottom_edge = top_edge + window.getSize().y;
//
//    // Calculate visible tile range, accounting for offsets
//    int start_x = static_cast<int>(floor((left_edge - offset_x) / tile_width_in_pixels_)) - 1;
//    int end_x = static_cast<int>(ceil((right_edge - offset_x) / tile_width_in_pixels_));
//    int start_y = static_cast<int>(floor((top_edge - offset_y) / tile_height_in_pixels_)) - 1;
//    int end_y = static_cast<int>(ceil((bottom_edge - offset_y) / tile_height_in_pixels_));
//
//    // Draw tiles
//    for (int i = start_x; i <= end_x; ++i) {
//        for (int j = start_y; j <= end_y; ++j) {
//            sf::RectangleShape rect(sf::Vector2f(tile_width_in_pixels_, tile_height_in_pixels_));
//            rect.setPosition(sf::Vector2f(offset_x + i * tile_width_in_pixels_, offset_y + j * tile_height_in_pixels_));
//
//            if (i < 0 || j < 0 || i >= Width() || j >= Height()) {
//                rect.setFillColor(sf::Color::Black); // Out of bounds
//            }
//            else if (char_map_[j][i] == '1') {
//                rect.setFillColor(sf::Color::Black); // Wall
//            }
//            else {
//                rect.setFillColor(sf::Color::White); // Open space
//            }
//
//            window.draw(rect);
//        }
//    }
//}

void GameMap::DrawMap(sf::RenderWindow& window, Player& player, double offset_x, double offset_y, candle::EdgeVector& edges) {
    // Reset edges vector
    edges.clear();

    // Calculate view edges
    float left_edge = player.getCenter().x - window.getSize().x / 2.0f;
    float top_edge = player.getCenter().y - window.getSize().y / 2.0f;
    float right_edge = left_edge + window.getSize().x;
    float bottom_edge = top_edge + window.getSize().y;

    // Calculate visible tile range, accounting for offsets
    int start_x = static_cast<int>(floor((left_edge - offset_x) / tile_width_in_pixels_)) - 1;
    int end_x = static_cast<int>(ceil((right_edge - offset_x) / tile_width_in_pixels_));
    int start_y = static_cast<int>(floor((top_edge - offset_y) / tile_height_in_pixels_)) - 1;
    int end_y = static_cast<int>(ceil((bottom_edge - offset_y) / tile_height_in_pixels_));

    // Helper function to check if a tile is a wall
    auto isWall = [&](int x, int y) -> bool {
        if (x < 0 || y < 0 || x >= Width() || y >= Height()) {
            return true; // Out of bounds treated as walls
        }
        return char_map_[y][x] == '1';
        };

    // Draw tiles first
    for (int i = start_x; i <= end_x; ++i) {
        for (int j = start_y; j <= end_y; ++j) {
            sf::RectangleShape rect(sf::Vector2f(tile_width_in_pixels_, tile_height_in_pixels_));
            float tile_left = offset_x + i * tile_width_in_pixels_;
            float tile_top = offset_y + j * tile_height_in_pixels_;
            rect.setPosition(sf::Vector2f(tile_left, tile_top));

            if (isWall(i, j)) {
                rect.setFillColor(sf::Color::Black); // Wall
            }
            else {
                rect.setFillColor(sf::Color::Transparent); // Open space
            }

            window.draw(rect);
        }
    }

    // Generate edges only for boundaries between walls and open spaces
    // We need a slightly larger range for edge detection to catch boundaries at view edges
    int edge_start_x = start_x - 1;
    int edge_end_x = end_x + 1;
    int edge_start_y = start_y - 1;
    int edge_end_y = end_y + 1;

    for (int i = edge_start_x; i <= edge_end_x; ++i) {
        for (int j = edge_start_y; j <= edge_end_y; ++j) {
            if (!isWall(i, j)) continue; // Only process wall tiles

            float tile_left = offset_x + i * tile_width_in_pixels_;
            float tile_top = offset_y + j * tile_height_in_pixels_;
            float tile_right = tile_left + tile_width_in_pixels_;
            float tile_bottom = tile_top + tile_height_in_pixels_;

            // Check each side of the wall tile and add edge if it borders open space

            // Left edge: if tile to the left is open space
            if (!isWall(i - 1, j)) {
                edges.emplace_back(sf::Vector2f(tile_left, tile_top),
                    sf::Vector2f(tile_left, tile_bottom));
            }

            // Right edge: if tile to the right is open space
            if (!isWall(i + 1, j)) {
                edges.emplace_back(sf::Vector2f(tile_right, tile_top),
                    sf::Vector2f(tile_right, tile_bottom));
            }

            // Top edge: if tile above is open space
            if (!isWall(i, j - 1)) {
                edges.emplace_back(sf::Vector2f(tile_left, tile_top),
                    sf::Vector2f(tile_right, tile_top));
            }

            // Bottom edge: if tile below is open space
            if (!isWall(i, j + 1)) {
                edges.emplace_back(sf::Vector2f(tile_left, tile_bottom),
                    sf::Vector2f(tile_right, tile_bottom));
            }
        }
    }
}

bool GameMap::IsOnBoundary(int i, int j) {
    return (char_map_[i + 1][j] == '1') || (char_map_[i - 1][j] == '1') ||
           (char_map_[i][j - 1] == '1') || (char_map_[i][j + 1] == '1');
}

int GameMap::Height() {
    return char_map_.size();
}

int GameMap::Width() {
    return char_map_[0].size();
}

std::vector<Room*> GameMap::GenerateRooms() {
    std::vector<Room*> rooms;
    std::vector<std::vector<int> > visited(Height(), std::vector<int>(Width(), 0));

    for (int i = 0; i < Height(); ++i) {
        for (int j = 0; j < Width(); ++j) {
            if (!visited[i][j] && (char_map_[i][j] == '0')) {
                std::vector<Node*> connected_component;
                Dfs(i, j, visited, connected_component);
                Room* new_room(new Room(connected_component, this));
                rooms.push_back(std::move(new_room));
            }
        }
    }

    return rooms;
}

int GameMap::CountConnectedComponents(std::vector<Room*>& rooms) {
    int cnt = 0;
    std::vector<std::vector<int> > visited(Height(), std::vector<int>(Width(), 0));
    std::vector<std::pair<int, int> > connected_component;
    for (int i = 0; i < Height(); ++i) {
        for (int j = 0; j < Width(); ++j) {
            if (!visited[i][j] && (char_map_[i][j] == '0')) {
                DfsSimple(i, j, visited, connected_component);
                ++cnt;
            }
        }
    }

    return cnt;
}

std::vector<std::vector<std::pair<int, int> > > GameMap::GroupTilesIntoConnectedComponenets(
    std::vector<Room*>& rooms) {
    std::vector<std::vector<std::pair<int, int> > > res;
    std::vector<std::vector<int> > visited(Height(), std::vector<int>(Width(), 0));
    for (int i = 0; i < Height(); ++i) {
        for (int j = 0; j < Width(); ++j) {
            if (!visited[i][j] && (char_map_[i][j] == '0')) {
                std::vector<std::pair<int, int> > cc;
                DfsSimple(i, j, visited, cc);
                res.push_back(cc);
            }
        }
    }

    return res;
}

void GameMap::RemoveConnectedComponents(std::vector<Room*>& rooms) {
    std::vector<std::vector<std::pair<int, int> > > connected_components =
        GroupTilesIntoConnectedComponenets(rooms);
    std::sort(connected_components.begin(), connected_components.end(),
        Util::CmpTileConnectedComponentsBySizeDescending);
    for (int i = 1; i < connected_components.size(); ++i) {
        for (auto& n : connected_components[i]) {
            char_map_[n.first][n.second] = '1';
        }
    }
}

void GameMap::Dfs(int i, int j, std::vector<std::vector<int> >& visited,
    std::vector<Node*>& connected_component) {
    visited[i][j] = 1;
    Node* new_node = new Node(j, i);
    connected_component.push_back(new_node);
    if ((IsValidCell(i + 1, j)) && !visited[i + 1][j] && (char_map_[i + 1][j] == '0')) {
        Dfs(i + 1, j, visited, connected_component);
    }
    if ((IsValidCell(i - 1, j)) && !visited[i - 1][j] && (char_map_[i - 1][j] == '0')) {
        Dfs(i - 1, j, visited, connected_component);
    }
    if ((IsValidCell(i, j + 1)) && !visited[i][j + 1] && (char_map_[i][j + 1] == '0')) {
        Dfs(i, j + 1, visited, connected_component);
    }
    if ((IsValidCell(i, j - 1)) && !visited[i][j - 1] && (char_map_[i][j - 1] == '0')) {
        Dfs(i, j - 1, visited, connected_component);
    }
}

void GameMap::DfsSimple(int i, int j, std::vector<std::vector<int> >& visited,
    std::vector<std::pair<int, int> >& connected_component) {
    visited[i][j] = 1;
    connected_component.push_back(std::make_pair(i, j));
    if ((IsValidCell(i + 1, j)) && !visited[i + 1][j] && (char_map_[i + 1][j] == '0')) {
        DfsSimple(i + 1, j, visited, connected_component);
    }
    if ((IsValidCell(i - 1, j)) && !visited[i - 1][j] && (char_map_[i - 1][j] == '0')) {
        DfsSimple(i - 1, j, visited, connected_component);
    }
    if ((IsValidCell(i, j + 1)) && !visited[i][j + 1] && (char_map_[i][j + 1] == '0')) {
        DfsSimple(i, j + 1, visited, connected_component);
    }
    if ((IsValidCell(i, j - 1)) && !visited[i][j - 1] && (char_map_[i][j - 1] == '0')) {
        DfsSimple(i, j - 1, visited, connected_component);
    }
}

void GameMap::DfsRoom(Room* room, std::vector<Room*>& rooms,
                      std::vector<Room*>& visited, std::vector<Room*>& connected_component) {

    if (std::find(visited.begin(), visited.end(), room) != visited.end()) {
        return;
    }

    visited.push_back(std::move(room));
    connected_component.push_back(room);
    for (auto& r : room->connected_rooms_) {
        DfsRoom(r, rooms, visited, connected_component);
    }
}

std::vector<std::vector<Room*> > GameMap::GroupRoomsIntoConnectedComponents(
    std::vector<Room*>& rooms) {
    std::vector<std::vector<Room*> > res;
    std::vector<Room*> visited;

    for (int i = 0; i < rooms.size(); ++i) {
        std::vector<Room*> connected_component;
        DfsRoom(rooms[i], rooms, visited, connected_component);
        if (!connected_component.empty()) {
            res.push_back(connected_component);
        }
    }

    return res;
}

void GameMap::ConnectMainRoomToOtherRooms(Room* main_room, std::vector<Room*>& rooms,
    std::vector<std::vector<Room*> >& rooms_connected_components) {
    for (auto& v : rooms_connected_components) {
        if (std::find(v.begin(), v.end(), main_room) == v.end()) {
            main_room->ConnectRoom(main_room->GetClosestRoom(v));
        }
    }
}

void GameMap::CreatePassageways() {
    std::vector<std::vector<Room*>> rooms_connected_components = 
        GroupRoomsIntoConnectedComponents(room_vector_);

    double offset_x = kOffsetX;
    double offset_y = kOffsetY;

    for (int i = 0; i < room_vector_.size(); ++i) {
        for (auto & r : room_vector_[i]->connected_rooms_) {
            if (room_vector_[i] == r) {
                continue;
            }
            std::pair<Node*, Node*> closest_nodes =
                room_vector_[i]->GetClosestNodes(r);

            double x0 = closest_nodes.first->x_;
            double y0 = closest_nodes.first->y_;
            x0 *= tile_width_in_pixels_;
            y0 *= tile_width_in_pixels_;
            x0 += offset_x;
            y0 += offset_y;

            double x1 = closest_nodes.second->x_;
            double y1 = closest_nodes.second->y_;
            x1 *= tile_width_in_pixels_;
            y1 *= tile_width_in_pixels_;
            x1 += offset_x;
            y1 += offset_y;

			double a = 0, b = 0, c = 0;
            if (fabs(x0 - x1) < kEpsilon) { // If the line is vertical, e.g. x = x0
                a = 1;
                b = 0;
                c = -x0;
            }
            else if (fabs(y0 - y1) < kEpsilon) { // If the line is vertical, e.g. x = x0
                a = 0;
                b = 1;
                c = -y0;
            }
            else {
                a = (y0 - y1) / (x0 - x1);
                b = 1;
                c = y0 - a * x0;
                a *= -1;
                c *= -1;
            }

            int cnt = 0;
            for (int i = 0; i < Height(); ++i) {
                for (int j = 0; j < Width(); ++j) {
                    double x = j * tile_width_in_pixels_ + tile_width_in_pixels_ / 2 + offset_x;
                    double y = i * tile_width_in_pixels_ + tile_width_in_pixels_ / 2 + offset_y;
                    double dist = Util::DistFromPointToLine(x, y, a, b, c);
                    if (dist < tile_width_in_pixels_ * kSqrt2) {
                        double dist_to_first_end = Util::DistSquaredFromPointToPoint(x, y, x0, y0);
                        double dist_to_second_end = Util::DistSquaredFromPointToPoint(x, y, x1, y1);
                        double dist_first_to_second = Util::DistSquaredFromPointToPoint(x0, y0, x1, y1);
                        if ((dist_to_first_end <= dist_first_to_second) &&
                            (dist_to_second_end <= dist_first_to_second)) {
                            char_map_[i][j] = '0';
                            ++cnt;
                        }
                    }
                }
            }
        }
    }
}

void GameMap::ConnectClosestRooms() {
    if (room_vector_.empty()) {
        std::cout << "In ConnectClosestRooms() function. ";
        std::cout << "Cannot connect rooms, because room list is empty." << std::endl;
        return;
    }
    if (room_vector_.size() == 1) {
        std::cout << "In ConnectClosestRooms() function. ";
        std::cout << " Cannot connect rooms, because room list contains "
                  << "only one room" << std::endl;
        return;
    }
    for (int i = 0; i < room_vector_.size(); ++i) {
        room_vector_[i]->ConnectRoom(room_vector_[i]->GetClosestRoom(room_vector_));
    }
}

void GameMap::ConnectRooms(std::vector<Room*>& rooms) {
    ConnectClosestRooms();
    sort(room_vector_.begin(), room_vector_.end(), Room::CmpRoomsBySize);
    if (!room_vector_.empty()) {
        room_vector_[0]->is_main_room_ = true;
    }
    std::vector<std::vector<Room*> > rooms_connected_components =
        GroupRoomsIntoConnectedComponents(room_vector_);
    if (!room_vector_.empty()) {
        ConnectMainRoomToOtherRooms(room_vector_[0], room_vector_, rooms_connected_components);
    }
    rooms_connected_components =
        GroupRoomsIntoConnectedComponents(room_vector_);
}

void GameMap::DrawRoomConnections(sf::RenderWindow& window) {
    for (int i = 0; i < room_vector_.size(); ++i) {
        Room* curr = room_vector_[i];
        for (auto& r : curr->connected_rooms_) {
            if (curr == r) {
                continue;
            }

            sf::VertexArray line(sf::LinesStrip, 2);
            int rand_index_0 = 0;
            int rand_index_1 = 0;

            line[0].position = sf::Vector2f(curr->node_vector_[rand_index_0]->x_ * kTileWidthInPixels + kOffsetX, curr->node_vector_[rand_index_0]->y_ * kTileWidthInPixels + kOffsetY);
            line[1].position = sf::Vector2f(r->node_vector_[rand_index_1]->x_ * kTileWidthInPixels + kOffsetX, r->node_vector_[rand_index_1]->y_ * kTileWidthInPixels + kOffsetY);
            line[0].color = sf::Color(255, 0, 0);
            line[1].color = sf::Color(255, 0, 0);

            window.draw(line);
        }
    }
}

void GameMap::CreateBoundaries() {
    for (int i = 0; i < Height(); ++i) {
        for (int j = 0; j < Width(); ++j) {
            if ((i == 0) || (i == Height() - 1) || (j == 0) || (j == Width() - 1)) {
                char_map_[i][j] = '1';
            }
        }
    }
}

int GameMap::CountNeighborsDist(int i, int j, char c, int dist) {
    int cnt = 0;
    for (int dx = -dist; dx < dist + 1; ++dx) {
        for (int dy = -dist; dy < dist + 1; ++dy) {
            if ((dx == 0) && (dy == 0)) {
                continue;
            }
            if (IsValidCell(i + dy, j + dx) && (char_map_[i + dy][j + dx] == c)) {
                ++cnt;
            }
        }
    }

    return cnt;
}

bool GameMap::isCollidingBullet(Bullet& bullet) {
    float playerX = bullet.getCenter().x - kOffsetX;
    float playerY = bullet.getCenter().y - kOffsetY;

    // Calculate the tile row and column based on the player's center
    int row = static_cast<int>(playerY / kTileHeightInPixels);
    int col = static_cast<int>(playerX / kTileWidthInPixels);

    // Check if the tile at the bullet's center is a wall
    if (char_map_[row][col] == '1') {
        return true; // Collision with a wall
    }

    return false; // No collision
}

bool GameMap::isCollidingPlayer(Player& player) {
    float playerX = player.getCenter().x - kOffsetX;
    float playerY = player.getCenter().y - kOffsetY;

    // Calculate the tile row and column based on the player's center
    int row = static_cast<int>(playerY / kTileHeightInPixels);
    int col = static_cast<int>(playerX / kTileWidthInPixels);

    // Check if the row and column are within bounds
    if (row < 0 || row >= Height() || col < 0 || col >= Width()) {
        return true; // Out of bounds, consider as colliding
    }

    // Check if the tile at the player's center is a wall
    if (char_map_[row][col] == '1') {
        return true; // Collision with a wall
    }

    return false; // No collision
}