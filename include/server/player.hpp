#pragma once

#include <string>
#include <map>
#include "../utils/types.hpp"
#include "../factions/faction_type.hpp"
#include "utils/color.hpp"

using PlayerID = std::uint32_t;

class Player
{
public:
    Player(PlayerID id, FactionType faction, Color color, float resource_handicap_percentage);

    PlayerID get_id() const { return id_; }
    FactionType get_faction() const { return faction_; }
    Color get_color() const { return color_; }
    float get_resource_handicap() const { return resource_handicap_percentage_; }
    const std::string &get_name() const { return name_; }
    void set_name(const std::string &name) { name_ = name; }
    bool is_ready() const { return ready_; }
    void set_ready(bool ready) { ready_ = ready; }

private:
    PlayerID id_;
    FactionType faction_;
    Color color_;
    float resource_handicap_percentage_;
    std::string name_;
    bool ready_{false};
};