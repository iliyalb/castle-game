#include "server/resource_manager.hpp"
#include <algorithm>

ResourceManager::ResourceManager()
{
    // Initialize default resources
    available_resources_ = {
        {1, "Gold", 1000, false, 0.0f},
        {2, "Wood", 500, true, 0.1f},
        {3, "Stone", 300, true, 0.05f},
        {4, "Food", 200, true, 0.2f}};
}

ResourceManager::~ResourceManager() = default;

void ResourceManager::add_resource(PlayerID player_id, const std::string &resource_name, int amount)
{
    player_resources_[player_id][resource_name] += amount;
}

bool ResourceManager::spend_resource(PlayerID player_id, const std::string &resource_name, int amount)
{
    auto &resources = player_resources_[player_id];
    if (resources[resource_name] >= amount)
    {
        resources[resource_name] -= amount;
        return true;
    }
    return false;
}

int ResourceManager::get_resource_amount(PlayerID player_id, const std::string &resource_name) const
{
    auto player_it = player_resources_.find(player_id);
    if (player_it != player_resources_.end())
    {
        auto resource_it = player_it->second.find(resource_name);
        if (resource_it != player_it->second.end())
        {
            return resource_it->second;
        }
    }
    return 0;
}

std::vector<Resource> ResourceManager::get_available_resources() const
{
    return available_resources_;
}

void ResourceManager::add_resource_node(int x, int y, const Resource &resource)
{
    resource_nodes_.push_back({x, y, resource, static_cast<float>(resource.initial_amount)});
}

void ResourceManager::remove_resource_node(int x, int y, int resource_id)
{
    resource_nodes_.erase(
        std::remove_if(resource_nodes_.begin(), resource_nodes_.end(),
                       [x, y, resource_id](const ResourceNode &node)
                       {
                           return node.x == x && node.y == y && node.resource.id == resource_id;
                       }),
        resource_nodes_.end());
}

void ResourceManager::update(float delta_time)
{
    for (auto &node : resource_nodes_)
    {
        if (node.resource.is_renewable && node.current_amount < node.resource.initial_amount)
        {
            node.current_amount += node.resource.respawn_rate * delta_time;
            node.current_amount = std::min(node.current_amount,
                                           static_cast<float>(node.resource.initial_amount));
        }
    }
}