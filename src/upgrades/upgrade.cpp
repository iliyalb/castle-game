#include "upgrades/upgrade.hpp"
#include "server/game_state.hpp"
#include <cmath>

Upgrade::Upgrade(const std::string &name, const std::string &description)
    : name_(name), description_(description)
{
}

void Upgrade::add_requirement(const UpgradeRequirement &req)
{
    requirements_.push_back(req);
}

void Upgrade::add_effect(const UpgradeEffect &effect)
{
    base_effects_.push_back(effect);
}

bool Upgrade::can_upgrade(PlayerID player_id) const
{
    if (current_level_ >= max_level_)
    {
        return false;
    }

    for (const auto &req : requirements_)
    {
        // Technology requirement check
        if (!req.technology_name.empty())
        {
            // Check if player has required technology
            // Implementation depends on how technology system is implemented
        }

        // Resource requirement check
        // Replace with actual resource checks when resource system is implemented
        [[maybe_unused]] bool has_resources = true;
        for (const auto &[resource, amount] : req.resource_costs)
        {
            // Check if player has required resources
            // has_resources &= player.has_resource(resource, amount);
        }
    }

    return true;
}

bool Upgrade::apply_upgrade(PlayerID player_id)
{
    if (!can_upgrade(player_id))
    {
        return false;
    }

    // Deduct resources
    for (const auto &req : requirements_)
    {
        for (const auto &[resource, amount] : req.resource_costs)
        {
            // Deduct resources from player
            // player.deduct_resource(resource, amount);
        }
    }

    current_level_++;
    return true;
}

std::vector<UpgradeEffect> Upgrade::get_effects_at_level(int level) const
{
    std::vector<UpgradeEffect> scaled_effects = base_effects_;
    float level_multiplier = std::pow(level_scaling_, level - 1);

    for (auto &effect : scaled_effects)
    {
        effect.modifier *= level_multiplier;
    }

    return scaled_effects;
}

float Upgrade::get_total_modifier(const std::string &attribute) const
{
    float total_modifier = 1.0f;
    auto effects = get_effects_at_level(current_level_);

    for (const auto &effect : effects)
    {
        if (effect.attribute == attribute)
        {
            if (effect.is_percentage)
            {
                total_modifier *= (1.0f + effect.modifier);
            }
            else
            {
                total_modifier += effect.modifier;
            }
        }
    }

    return total_modifier;
}