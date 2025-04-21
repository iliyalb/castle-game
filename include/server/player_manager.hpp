#pragma once

#include <map>
#include <vector>
#include <memory>
#include <algorithm>
#include "../utils/types.hpp"
#include "../utils/color.hpp"
#include "../factions/faction.hpp"
#include "player.hpp"

using PlayerID = std::uint32_t;
class GameState;    // Forward declaration
class CastleServer; // Forward declaration

class PlayerManager
{
public:
    PlayerManager();
    ~PlayerManager();

    std::shared_ptr<Player> create_player(const Faction &faction, Color color, float resource_handicap_percentage);
    void remove_player(PlayerID player_id);
    std::shared_ptr<Player> get_player(PlayerID player_id);
    void assign_to_team(PlayerID player_id, TeamNumber team);
    std::vector<PlayerID> get_players_in_team(TeamNumber team) const;
    PlayerID generate_next_player_id() { return next_player_id_++; }

    friend class CastleServer;

private:
    std::map<PlayerID, std::shared_ptr<Player>> players_;
    std::map<TeamNumber, std::vector<PlayerID>> team_assignments_;
    PlayerID next_player_id_{1}; // Start from 1, 0 can be reserved for invalid/system
};