#pragma once

#include "unit.hpp"
#include "utils/types.hpp"

// Forward declaration of UnitStats to avoid circular dependency
class UnitStats;

class Peasant : public Unit
{
public:
    explicit Peasant(PlayerID owner_id);
    bool can_harvest() const override { return true; }
    static UnitStats get_base_stats();
};

class Soldier : public Unit
{
public:
    explicit Soldier(PlayerID owner_id);
    static UnitStats get_base_stats();
};

class Archer : public Unit
{
public:
    explicit Archer(PlayerID owner_id);
    static UnitStats get_base_stats();
};

class Knight : public Unit
{
public:
    explicit Knight(PlayerID owner_id);
    static UnitStats get_base_stats();
};

class Healer : public Unit
{
public:
    explicit Healer(PlayerID owner_id);
    bool can_attack() const override { return false; }
    void heal_target(Unit *target);
    static UnitStats get_base_stats();
};

class Scout : public Unit
{
public:
    explicit Scout(PlayerID owner_id);
    static UnitStats get_base_stats();
};