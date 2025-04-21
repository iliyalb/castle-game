#pragma once

#include <memory>
#include <map>
#include <string>
#include <vector>
#include "../utils/types.hpp"
#include "upgrade.hpp"
#include "specific_upgrades.hpp"
#include "../server/player.hpp"

struct PlayerUpgrades
{
    std::map<std::string, std::unique_ptr<Upgrade>> upgrades;
    std::vector<std::string> completed_technologies;
};

class UpgradeManager
{
public:
    UpgradeManager();
    ~UpgradeManager() = default;

    // Upgrade operations
    bool purchase_upgrade(PlayerID player_id, const std::string &upgrade_name);
    bool can_purchase_upgrade(PlayerID player_id, const std::string &upgrade_name) const;
    float get_modifier(PlayerID player_id, const std::string &upgrade_name, const std::string &attribute) const;

    // Technology management
    bool unlock_technology(PlayerID player_id, const std::string &tech_name);
    bool has_technology(PlayerID player_id, const std::string &tech_name) const;
    std::vector<std::string> get_available_technologies(PlayerID player_id) const;

    // Get upgrade information
    std::vector<std::string> get_available_upgrades(PlayerID player_id) const;
    int get_upgrade_level(PlayerID player_id, const std::string &upgrade_name) const;

private:
    void initialize_player_upgrades(PlayerID player_id);
    std::unique_ptr<Upgrade> create_upgrade(const std::string &upgrade_name) const;

    std::map<PlayerID, PlayerUpgrades> player_upgrades_;
};