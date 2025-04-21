#include "factions/faction.hpp"
#include <algorithm>

Faction::Faction(const std::string &name, const std::string &description)
    : name_(name), description_(description)
{
    // Default available units for all factions
    available_units_ = {
        UnitType::Peasant,
        UnitType::Soldier};
}

void Faction::add_bonus(const FactionBonus &bonus)
{
    bonuses_[bonus.name] = bonus;
}

void Faction::add_unit_modifier(const UnitModifier &modifier)
{
    unit_modifiers_[modifier.unit_type] = modifier;
}

float Faction::get_bonus_value(const std::string &bonus_name) const
{
    auto it = bonuses_.find(bonus_name);
    return it != bonuses_.end() ? it->second.value : 1.0f;
}

const UnitModifier *Faction::get_unit_modifier(UnitType type) const
{
    auto it = unit_modifiers_.find(type);
    return it != unit_modifiers_.end() ? &it->second : nullptr;
}

std::unique_ptr<Unit> Faction::create_unit(UnitType type, PlayerID owner_id)
{
    if (!can_build_unit(type))
    {
        return nullptr;
    }

    std::unique_ptr<Unit> unit;
    switch (type)
    {
    case UnitType::Peasant:
        unit = std::make_unique<Peasant>(owner_id);
        break;
    case UnitType::Soldier:
        unit = std::make_unique<Soldier>(owner_id);
        break;
    case UnitType::Archer:
        unit = std::make_unique<Archer>(owner_id);
        break;
    case UnitType::Knight:
        unit = std::make_unique<Knight>(owner_id);
        break;
    case UnitType::Healer:
        unit = std::make_unique<Healer>(owner_id);
        break;
    case UnitType::Scout:
        unit = std::make_unique<Scout>(owner_id);
        break;
    default:
        return nullptr;
    }

    // Apply faction modifiers
    if (unit)
    {
        if (const UnitModifier *modifier = get_unit_modifier(type))
        {
            UnitStats modified_stats = unit->get_stats();
            modified_stats.max_health = static_cast<int>(modified_stats.max_health * modifier->health_modifier);
            modified_stats.attack = static_cast<int>(modified_stats.attack * modifier->attack_modifier);
            modified_stats.armor = static_cast<int>(modified_stats.armor * modifier->defense_modifier);
            modified_stats.speed = static_cast<int>(modified_stats.speed * modifier->speed_modifier);
            unit->modify_stats(modified_stats);
        }
    }

    return unit;
}

bool Faction::can_build_unit(UnitType type) const
{
    return std::find(available_units_.begin(), available_units_.end(), type) != available_units_.end();
}

bool Faction::has_technology(const std::string &tech_name) const
{
    return std::find(unlocked_technologies_.begin(), unlocked_technologies_.end(), tech_name) != unlocked_technologies_.end();
}

void Faction::unlock_technology(const std::string &tech_name)
{
    if (!has_technology(tech_name))
    {
        unlocked_technologies_.push_back(tech_name);
    }
}