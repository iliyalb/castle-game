#pragma once

#include <string>
#include <vector>
#include <map>
#include "../utils/types.hpp"
#include "../units/unit.hpp"
#include "../server/player.hpp"

struct UpgradeRequirement
{
    std::string technology_name;
    std::vector<std::pair<std::string, int>> resource_costs;
    int required_level{1};
};

struct UpgradeEffect
{
    std::string attribute;
    float modifier;
    bool is_percentage{true};
};

class Upgrade
{
public:
    Upgrade(const std::string &name, const std::string &description);
    virtual ~Upgrade() = default;

    // Basic properties
    const std::string &get_name() const { return name_; }
    const std::string &get_description() const { return description_; }
    int get_level() const { return current_level_; }
    int get_max_level() const { return max_level_; }

    // Requirements and effects
    void add_requirement(const UpgradeRequirement &req);
    void add_effect(const UpgradeEffect &effect);
    bool can_upgrade(PlayerID player_id) const;
    bool apply_upgrade(PlayerID player_id);

    // Get upgrade effects at specific level
    std::vector<UpgradeEffect> get_effects_at_level(int level) const;
    float get_total_modifier(const std::string &attribute) const;
    float get_level_scaling() const { return level_scaling_; }

protected:
    void set_level_scaling(float scaling) { level_scaling_ = scaling; }

    std::string name_;
    std::string description_;
    int current_level_{0};
    int max_level_{3};
    std::vector<UpgradeRequirement> requirements_;
    std::vector<UpgradeEffect> base_effects_;
    float level_scaling_{1.5f}; // Multiplier for effects per level
};