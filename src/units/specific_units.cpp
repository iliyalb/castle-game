#include "units/specific_units.hpp"
#include "units/unit_stats.hpp"

// Peasant implementation
UnitStats Peasant::get_base_stats()
{
    return UnitStats(100, 5, 0, 3, 50, 0); // Low combat stats, cheap
}

Peasant::Peasant(PlayerID owner_id)
    : Unit(UnitType::Peasant, owner_id, get_base_stats()) {}

// Soldier implementation
UnitStats Soldier::get_base_stats()
{
    return UnitStats(150, 12, 2, 3, 100, 0); // Balanced combat unit
}

Soldier::Soldier(PlayerID owner_id)
    : Unit(UnitType::Soldier, owner_id, get_base_stats()) {}

// Archer implementation
UnitStats Archer::get_base_stats()
{
    return UnitStats(100, 15, 0, 3, 125, 25); // High attack, low defense
}

Archer::Archer(PlayerID owner_id)
    : Unit(UnitType::Archer, owner_id, get_base_stats()) {}

// Knight implementation
UnitStats Knight::get_base_stats()
{
    return UnitStats(200, 18, 4, 4, 200, 50); // Strong all-around
}

Knight::Knight(PlayerID owner_id)
    : Unit(UnitType::Knight, owner_id, get_base_stats()) {}

// Healer implementation
UnitStats Healer::get_base_stats()
{
    return UnitStats(100, 0, 1, 2, 150, 50); // Support unit
}

Healer::Healer(PlayerID owner_id)
    : Unit(UnitType::Healer, owner_id, get_base_stats()) {}

void Healer::heal_target(Unit *target)
{
    if (target && target->get_health() < target->get_max_health())
    {
        int heal_amount = 20;              // Base healing amount
        target->take_damage(-heal_amount); // Negative damage = healing
    }
}

// Scout implementation
UnitStats Scout::get_base_stats()
{
    return UnitStats(75, 8, 0, 5, 100, 25); // Fast, fragile
}

Scout::Scout(PlayerID owner_id)
    : Unit(UnitType::Scout, owner_id, get_base_stats()) {}