#pragma once

#include "utils/types.hpp"
#include "unit_stats.hpp"
#include <vector>

// Forward declarations to break circular dependencies
class Player;
class UnitStats;
class Ability;

enum class UnitType
{
    // Basic units
    Peasant,
    Soldier,
    Archer,
    Cavalry,

    // Advanced units
    Knight,
    Siege,
    Healer,

    // Special units
    Hero,
    Scout
};

class Unit
{
protected:
    UnitType unit_type;
    Player *owner;
    int x;
    int y;
    UnitStats stats;
    int current_health;

public:
    Unit(UnitType type, PlayerID owner_id, const UnitStats &initial_stats);
    virtual ~Unit() = default;

    virtual void move(int dx, int dy);
    virtual void attack(Unit *target);
    virtual void take_damage(int damage);
    virtual bool can_attack() const { return true; }
    virtual bool can_harvest() const { return false; }

    const UnitStats &get_stats() const { return stats; }
    void modify_stats(const UnitStats &new_stats)
    {
        stats = new_stats;
        current_health = std::min(current_health, stats.max_health);
    }

    Player *get_owner() const { return owner; }
    UnitType get_unit_type() const { return unit_type; }
    int get_health() const { return current_health; }
    int get_max_health() const { return stats.max_health; }
    int get_attack() const { return stats.attack; }
    int get_armor() const { return stats.armor; }
    int get_speed() const { return stats.speed; }
    void set_position(int new_x, int new_y)
    {
        x = new_x;
        y = new_y;
    }
    void get_position(int &out_x, int &out_y) const
    {
        out_x = x;
        out_y = y;
    }
};