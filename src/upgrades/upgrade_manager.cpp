#include "upgrades/upgrade_manager.hpp"
#include <algorithm>

UpgradeManager::UpgradeManager() {}

void UpgradeManager::initialize_player_upgrades(PlayerID player_id)
{
    if (player_upgrades_.find(player_id) != player_upgrades_.end())
    {
        return;
    }

    PlayerUpgrades upgrades;
    // Initialize with all available upgrade types
    upgrades.upgrades["weapon"] = std::make_unique<WeaponUpgrade>();
    upgrades.upgrades["armor"] = std::make_unique<ArmorUpgrade>();
    upgrades.upgrades["training"] = std::make_unique<TrainingUpgrade>();
    upgrades.upgrades["resource"] = std::make_unique<ResourceUpgrade>();
    upgrades.upgrades["defense"] = std::make_unique<DefenseUpgrade>();

    player_upgrades_[player_id] = std::move(upgrades);
}

std::unique_ptr<Upgrade> UpgradeManager::create_upgrade(const std::string &upgrade_name) const
{
    if (upgrade_name == "weapon")
        return std::make_unique<WeaponUpgrade>();
    if (upgrade_name == "armor")
        return std::make_unique<ArmorUpgrade>();
    if (upgrade_name == "training")
        return std::make_unique<TrainingUpgrade>();
    if (upgrade_name == "resource")
        return std::make_unique<ResourceUpgrade>();
    if (upgrade_name == "defense")
        return std::make_unique<DefenseUpgrade>();
    return nullptr;
}

bool UpgradeManager::purchase_upgrade(PlayerID player_id, const std::string &upgrade_name)
{
    initialize_player_upgrades(player_id);
    auto &player_data = player_upgrades_[player_id];

    auto upgrade_it = player_data.upgrades.find(upgrade_name);
    if (upgrade_it == player_data.upgrades.end())
    {
        return false;
    }

    return upgrade_it->second->apply_upgrade(player_id);
}

bool UpgradeManager::can_purchase_upgrade(PlayerID player_id, const std::string &upgrade_name) const
{
    auto player_it = player_upgrades_.find(player_id);
    if (player_it == player_upgrades_.end())
    {
        return false;
    }

    auto upgrade_it = player_it->second.upgrades.find(upgrade_name);
    if (upgrade_it == player_it->second.upgrades.end())
    {
        return false;
    }

    return upgrade_it->second->can_upgrade(player_id);
}

float UpgradeManager::get_modifier(PlayerID player_id, const std::string &upgrade_name, const std::string &attribute) const
{
    auto player_it = player_upgrades_.find(player_id);
    if (player_it == player_upgrades_.end())
    {
        return 1.0f;
    }

    auto upgrade_it = player_it->second.upgrades.find(upgrade_name);
    if (upgrade_it == player_it->second.upgrades.end())
    {
        return 1.0f;
    }

    return upgrade_it->second->get_total_modifier(attribute);
}

bool UpgradeManager::unlock_technology(PlayerID player_id, const std::string &tech_name)
{
    initialize_player_upgrades(player_id);
    auto &technologies = player_upgrades_[player_id].completed_technologies;

    if (std::find(technologies.begin(), technologies.end(), tech_name) != technologies.end())
    {
        return false; // Already unlocked
    }

    technologies.push_back(tech_name);
    return true;
}

bool UpgradeManager::has_technology(PlayerID player_id, const std::string &tech_name) const
{
    auto player_it = player_upgrades_.find(player_id);
    if (player_it == player_upgrades_.end())
    {
        return false;
    }

    const auto &technologies = player_it->second.completed_technologies;
    return std::find(technologies.begin(), technologies.end(), tech_name) != technologies.end();
}

std::vector<std::string> UpgradeManager::get_available_technologies(PlayerID player_id) const
{
    std::vector<std::string> available = {
        "Basic Smithing",
        "Basic Armory",
        "Military Tactics",
        "Economic Development",
        "Construction Mastery"};

    // Filter out already completed technologies
    auto player_it = player_upgrades_.find(player_id);
    if (player_it != player_upgrades_.end())
    {
        const auto &completed = player_it->second.completed_technologies;
        available.erase(
            std::remove_if(available.begin(), available.end(),
                           [&completed](const std::string &tech)
                           {
                               return std::find(completed.begin(), completed.end(), tech) != completed.end();
                           }),
            available.end());
    }

    return available;
}

std::vector<std::string> UpgradeManager::get_available_upgrades(PlayerID player_id) const
{
    std::vector<std::string> available;
    auto player_it = player_upgrades_.find(player_id);
    if (player_it != player_upgrades_.end())
    {
        for (const auto &[name, upgrade] : player_it->second.upgrades)
        {
            if (upgrade->can_upgrade(player_id))
            {
                available.push_back(name);
            }
        }
    }
    return available;
}

int UpgradeManager::get_upgrade_level(PlayerID player_id, const std::string &upgrade_name) const
{
    auto player_it = player_upgrades_.find(player_id);
    if (player_it == player_upgrades_.end())
    {
        return 0;
    }

    auto upgrade_it = player_it->second.upgrades.find(upgrade_name);
    if (upgrade_it == player_it->second.upgrades.end())
    {
        return 0;
    }

    return upgrade_it->second->get_level();
}