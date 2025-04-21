#include "units/unit.hpp"
#include "server/player.hpp"

Unit::Unit(UnitType type, PlayerID owner_id, const UnitStats &initial_stats)
    : unit_type(type), stats(initial_stats), current_health(initial_stats.max_health)
{
}

void Unit::take_damage(int damage)
{
    // Apply armor reduction
    int actual_damage = std::max(1, damage - stats.armor);

    // Cannot heal through damage
    if (damage > 0)
    {
        current_health = std::max(0, current_health - actual_damage);
    }
    else
    {
        // Healing
        current_health = std::min(stats.max_health, current_health - damage);
    }
}

void Unit::move(int dx, int dy)
{
    x += dx * stats.speed;
    y += dy * stats.speed;
}

void Unit::attack(Unit *target)
{
    if (target && can_attack())
    {
        target->take_damage(stats.attack);
    }
}