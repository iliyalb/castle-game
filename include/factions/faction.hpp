#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "../utils/types.hpp"
#include "../units/unit.hpp"
#include "../units/specific_units.hpp"
#include "faction_type.hpp"

struct FactionBonus
{
    std::string name;
    std::string description;
    float value;
};

struct UnitModifier
{
    UnitType unit_type;
    float health_modifier{1.0f};
    float attack_modifier{1.0f};
    float defense_modifier{1.0f};
    float speed_modifier{1.0f};
    float cost_modifier{1.0f};
};

class Faction
{
public:
    Faction(const std::string &name, const std::string &description);
    virtual ~Faction() = default;

    // Basic properties
    const std::string &get_name() const { return name_; }
    const std::string &get_description() const { return description_; }
    virtual FactionType get_type() const = 0;

    // Bonuses and modifiers
    void add_bonus(const FactionBonus &bonus);
    void add_unit_modifier(const UnitModifier &modifier);
    float get_bonus_value(const std::string &bonus_name) const;
    const UnitModifier *get_unit_modifier(UnitType type) const;

    // Unit creation with faction modifiers
    virtual std::unique_ptr<Unit> create_unit(UnitType type, PlayerID owner_id);

    // Available units and technologies
    bool can_build_unit(UnitType type) const;
    bool has_technology(const std::string &tech_name) const;
    void unlock_technology(const std::string &tech_name);

protected:
    std::string name_;
    std::string description_;
    std::map<std::string, FactionBonus> bonuses_;
    std::map<UnitType, UnitModifier> unit_modifiers_;
    std::vector<UnitType> available_units_;
    std::vector<std::string> unlocked_technologies_;
};