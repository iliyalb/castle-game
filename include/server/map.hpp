#pragma once

#include <vector>
#include <memory>
#include <string>

enum class TerrainType
{
    Plains,
    Forest,
    Mountain,
    Water,
    Desert
};

struct Tile
{
    TerrainType terrain;
    bool walkable;
    bool buildable;
    float movement_cost;
};

class Map
{
public:
    Map(int width, int height, int tile_size);
    ~Map();

    bool load_from_file(const std::string &filename);
    bool save_to_file(const std::string &filename) const;

    Tile *get_tile(int x, int y);
    const Tile *get_tile(int x, int y) const;
    bool is_walkable(int x, int y) const;
    bool is_buildable(int x, int y) const;
    float get_movement_cost(int x, int y) const;

    int get_width() const { return width_; }
    int get_height() const { return height_; }
    int get_tile_size() const { return tile_size_; }

private:
    int width_;
    int height_;
    int tile_size_;
    std::vector<Tile> tiles_;

    bool is_valid_position(int x, int y) const;
};