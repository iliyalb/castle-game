#pragma once

#include <string>
#include <map>
#include <memory>
#include <vector>
#include "../utils/types.hpp"
#include "game_state.hpp"

struct Resource
{
    int id;
    std::string name;
    int initial_amount;
    bool is_renewable;
    float respawn_rate;
};

class ResourceManager
{
public:
    ResourceManager();
    ~ResourceManager();

    void add_resource(PlayerID player_id, const std::string &resource_name, int amount);
    bool spend_resource(PlayerID player_id, const std::string &resource_name, int amount);
    int get_resource_amount(PlayerID player_id, const std::string &resource_name) const;
    std::vector<Resource> get_available_resources() const;

    void add_resource_node(int x, int y, const Resource &resource);
    void remove_resource_node(int x, int y, int resource_id);
    void update(float delta_time); // For resource regeneration

private:
    std::map<PlayerID, std::map<std::string, int>> player_resources_;
    std::vector<Resource> available_resources_;
    struct ResourceNode
    {
        int x, y;
        Resource resource;
        float current_amount;
    };
    std::vector<ResourceNode> resource_nodes_;
};