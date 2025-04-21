#pragma once

#include <map>
#include <cstdint>
#include <memory>
#include <vector>
#include "../utils/types.hpp"
#include "player_manager.hpp"
#include "../factions/faction.hpp"
#include "../upgrades/upgrade_manager.hpp"

using PlayerID = std::uint32_t;

enum class VictoryState
{
    None,
    Player1Win,
    Player2Win,
    Draw
};

class GameState
{
public:
    GameState();
    ~GameState() = default;

    VictoryState get_victory_state() const { return victory_state_; }
    void set_victory_state(VictoryState state) { victory_state_ = state; }

    void update_player_score(PlayerID player_id, int score);
    int get_player_score(PlayerID player_id) const;

    bool is_game_over() const { return is_game_over_; }
    void set_game_over(bool game_over) { is_game_over_ = game_over; }

    std::int64_t get_elapsed_time() const { return elapsed_time_; }
    void update_elapsed_time(std::int64_t delta) { elapsed_time_ += delta; }

    // Player management
    PlayerID add_player(const std::string &name, const Faction &faction);
    void remove_player(PlayerID id);
    std::shared_ptr<Player> get_player(PlayerID id);

    // Game state checks
    bool is_game_started() const { return game_started_; }
    void set_game_started(bool started) { game_started_ = started; }

    // Upgrade management
    UpgradeManager &get_upgrade_manager() { return *upgrade_manager_; }
    const UpgradeManager &get_upgrade_manager() const { return *upgrade_manager_; }

private:
    VictoryState victory_state_{VictoryState::None};
    std::map<PlayerID, int> player_scores_;
    std::int64_t elapsed_time_{0};
    bool is_game_over_{false};
    std::unique_ptr<PlayerManager> player_manager_;
    std::unique_ptr<UpgradeManager> upgrade_manager_;
    bool game_started_{false};
};