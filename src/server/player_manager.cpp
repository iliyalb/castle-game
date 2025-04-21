#include "server/player_manager.hpp"
#include "server/player.hpp"
#include "utils/color.hpp"
#include <algorithm>

PlayerManager::PlayerManager() = default;
PlayerManager::~PlayerManager() = default;

std::shared_ptr<Player> PlayerManager::create_player(const Faction &faction, Color color, float resource_handicap_percentage)
{
    PlayerID id = next_player_id_++;
    auto player = std::make_shared<Player>(id, faction.get_type(), color, resource_handicap_percentage);
    players_[id] = player;
    return player;
}

void PlayerManager::remove_player(PlayerID player_id)
{
    players_.erase(player_id);

    // Remove from teams
    for (auto &[team, players] : team_assignments_)
    {
        auto it = std::find(players.begin(), players.end(), player_id);
        if (it != players.end())
        {
            players.erase(it);
        }
    }
}

std::shared_ptr<Player> PlayerManager::get_player(PlayerID player_id)
{
    auto it = players_.find(player_id);
    return it != players_.end() ? it->second : nullptr;
}

void PlayerManager::assign_to_team(PlayerID player_id, TeamNumber team)
{
    // Remove from any existing team
    for (auto &[team_num, players] : team_assignments_)
    {
        auto it = std::find(players.begin(), players.end(), player_id);
        if (it != players.end())
        {
            players.erase(it);
        }
    }

    // Add to new team
    team_assignments_[team].push_back(player_id);
}

std::vector<PlayerID> PlayerManager::get_players_in_team(TeamNumber team) const
{
    auto it = team_assignments_.find(team);
    return it != team_assignments_.end() ? it->second : std::vector<PlayerID>();
}