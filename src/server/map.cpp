#include "server/map.hpp"
#include <fstream>
#include <stdexcept>
#include <limits>

Map::Map(int width, int height, int tile_size)
    : width_(width), height_(height), tile_size_(tile_size)
{
    tiles_.resize(width * height);
    // Initialize with default plain tiles
    for (auto &tile : tiles_)
    {
        tile = {TerrainType::Plains, true, true, 1.0f};
    }
}

Map::~Map() = default;

bool Map::load_from_file(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        return false;
    }

    file.read(reinterpret_cast<char *>(&width_), sizeof(width_));
    file.read(reinterpret_cast<char *>(&height_), sizeof(height_));
    file.read(reinterpret_cast<char *>(&tile_size_), sizeof(tile_size_));

    tiles_.resize(width_ * height_);
    file.read(reinterpret_cast<char *>(tiles_.data()), tiles_.size() * sizeof(Tile));

    return true;
}

bool Map::save_to_file(const std::string &filename) const
{
    std::ofstream file(filename, std::ios::binary);
    if (!file)
    {
        return false;
    }

    file.write(reinterpret_cast<const char *>(&width_), sizeof(width_));
    file.write(reinterpret_cast<const char *>(&height_), sizeof(height_));
    file.write(reinterpret_cast<const char *>(&tile_size_), sizeof(tile_size_));
    file.write(reinterpret_cast<const char *>(tiles_.data()), tiles_.size() * sizeof(Tile));

    return true;
}

Tile *Map::get_tile(int x, int y)
{
    return is_valid_position(x, y) ? &tiles_[y * width_ + x] : nullptr;
}

const Tile *Map::get_tile(int x, int y) const
{
    return is_valid_position(x, y) ? &tiles_[y * width_ + x] : nullptr;
}

bool Map::is_walkable(int x, int y) const
{
    const Tile *tile = get_tile(x, y);
    return tile ? tile->walkable : false;
}

bool Map::is_buildable(int x, int y) const
{
    const Tile *tile = get_tile(x, y);
    return tile ? tile->buildable : false;
}

float Map::get_movement_cost(int x, int y) const
{
    const Tile *tile = get_tile(x, y);
    return tile ? tile->movement_cost : std::numeric_limits<float>::infinity();
}

bool Map::is_valid_position(int x, int y) const
{
    return x >= 0 && x < width_ && y >= 0 && y < height_;
}