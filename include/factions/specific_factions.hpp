#pragma once

#include "faction.hpp"
#include "../server/player.hpp" // For FactionType enum

class KingdomFaction : public Faction
{
public:
    KingdomFaction();
    FactionType get_type() const override { return FactionType::Kingdom; }

private:
    void initialize_bonuses();
    void initialize_unit_modifiers();
};

class EmpireFaction : public Faction
{
public:
    EmpireFaction();
    FactionType get_type() const override { return FactionType::Empire; }

private:
    void initialize_bonuses();
    void initialize_unit_modifiers();
};

class RepublicFaction : public Faction
{
public:
    RepublicFaction();
    FactionType get_type() const override { return FactionType::Republic; }

private:
    void initialize_bonuses();
    void initialize_unit_modifiers();
};

class NomadsFaction : public Faction
{
public:
    NomadsFaction();
    FactionType get_type() const override { return FactionType::Nomads; }

private:
    void initialize_bonuses();
    void initialize_unit_modifiers();
};