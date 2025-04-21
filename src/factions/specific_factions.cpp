#include "factions/specific_factions.hpp"

// Kingdom Faction
KingdomFaction::KingdomFaction()
    : Faction("Kingdom", "A traditional medieval kingdom with strong cavalry and defensive capabilities")
{
    initialize_bonuses();
    initialize_unit_modifiers();

    // Kingdom-specific available units
    available_units_ = {
        UnitType::Peasant,
        UnitType::Soldier,
        UnitType::Knight,
        UnitType::Archer,
        UnitType::Healer};
}

void KingdomFaction::initialize_bonuses()
{
    add_bonus({"defensive_structures", "Stronger defensive structures", 1.2f});
    add_bonus({"cavalry_power", "Increased cavalry effectiveness", 1.3f});
    add_bonus({"resource_storage", "Increased resource storage capacity", 1.25f});
}

void KingdomFaction::initialize_unit_modifiers()
{
    add_unit_modifier({UnitType::Knight, 1.2f, 1.2f, 1.2f, 1.1f, 1.0f});
    add_unit_modifier({UnitType::Peasant, 1.1f, 1.0f, 1.1f, 1.0f, 0.9f});
}

// Empire Faction
EmpireFaction::EmpireFaction()
    : Faction("Empire", "A powerful empire with advanced military technology and organized structure")
{
    initialize_bonuses();
    initialize_unit_modifiers();

    available_units_ = {
        UnitType::Peasant,
        UnitType::Soldier,
        UnitType::Archer,
        UnitType::Knight,
        UnitType::Scout};
}

void EmpireFaction::initialize_bonuses()
{
    add_bonus({"military_training", "Better trained military units", 1.15f});
    add_bonus({"resource_efficiency", "More efficient resource gathering", 1.2f});
    add_bonus({"technology_cost", "Reduced technology costs", 0.85f});
}

void EmpireFaction::initialize_unit_modifiers()
{
    add_unit_modifier({UnitType::Soldier, 1.15f, 1.2f, 1.15f, 1.0f, 1.1f});
    add_unit_modifier({UnitType::Archer, 1.1f, 1.15f, 1.0f, 1.1f, 1.1f});
}

// Republic Faction
RepublicFaction::RepublicFaction()
    : Faction("Republic", "A progressive society focused on technology and economic growth")
{
    initialize_bonuses();
    initialize_unit_modifiers();

    available_units_ = {
        UnitType::Peasant,
        UnitType::Soldier,
        UnitType::Archer,
        UnitType::Healer,
        UnitType::Scout};
}

void RepublicFaction::initialize_bonuses()
{
    add_bonus({"research_speed", "Faster research and development", 1.3f});
    add_bonus({"trade_efficiency", "Better trade rates", 1.25f});
    add_bonus({"population_growth", "Faster population growth", 1.2f});
}

void RepublicFaction::initialize_unit_modifiers()
{
    add_unit_modifier({UnitType::Peasant, 1.0f, 1.0f, 1.0f, 1.2f, 0.85f});
    add_unit_modifier({UnitType::Scout, 1.1f, 1.0f, 1.0f, 1.3f, 0.9f});
}

// Nomads Faction
NomadsFaction::NomadsFaction()
    : Faction("Nomads", "A mobile civilization skilled in raids and quick strikes")
{
    initialize_bonuses();
    initialize_unit_modifiers();

    available_units_ = {
        UnitType::Peasant,
        UnitType::Scout,
        UnitType::Archer,
        UnitType::Soldier,
        UnitType::Healer};
}

void NomadsFaction::initialize_bonuses()
{
    add_bonus({"mobility", "Increased movement speed", 1.3f});
    add_bonus({"raid_efficiency", "Better raiding capabilities", 1.25f});
    add_bonus({"adaptability", "Better resource gathering from varied sources", 1.2f});
}

void NomadsFaction::initialize_unit_modifiers()
{
    add_unit_modifier({UnitType::Scout, 1.2f, 1.15f, 1.0f, 1.3f, 0.9f});
    add_unit_modifier({UnitType::Archer, 1.1f, 1.2f, 0.9f, 1.2f, 1.0f});
}