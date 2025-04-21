#include "server/game_state.hpp"
#include "server/player_manager.hpp"
#include "upgrades/upgrade_manager.hpp"

GameState::GameState()
    : player_manager_(std::make_unique<PlayerManager>()),
      upgrade_manager_(std::make_unique<UpgradeManager>())
{
}

PlayerID GameState::add_player(const std::string &name, const Faction &faction)
{
    PlayerID id = player_manager_->generate_next_player_id();
    auto player = player_manager_->create_player(faction, Color{255, 255, 255, 255}, 1.0f);
    player->set_name(name);
    return id;
}

void GameState::remove_player(PlayerID id)
{
    player_manager_->remove_player(id);
}

std::shared_ptr<Player> GameState::get_player(PlayerID id)
{
    return player_manager_->get_player(id);
}

void GameState::update_player_score(PlayerID player_id, int score)
{
    player_scores_[player_id] = score;
}

int GameState::get_player_score(PlayerID player_id) const
{
    auto it = player_scores_.find(player_id);
    return it != player_scores_.end() ? it->second : 0;
}