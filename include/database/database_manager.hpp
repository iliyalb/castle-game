#pragma once

#include <sqlite3.h>
#include <string>
#include <memory>
#include <vector>
#include "../utils/types.hpp"
#include "../server/player.hpp"

struct MatchRecord
{
    uint32_t match_id;
    std::int64_t start_time;
    std::int64_t end_time;
    std::vector<PlayerID> winners;
    std::vector<PlayerID> losers;
};

struct PlayerStats
{
    PlayerID player_id;
    int matches_played;
    int matches_won;
    int total_resources_gathered;
    int total_units_created;
    int total_buildings_constructed;
};

class DatabaseManager
{
public:
    DatabaseManager(const std::string &db_path);
    ~DatabaseManager();

    bool initialize();

    // Player management
    bool save_player(const Player &player);
    bool load_player(PlayerID id, Player &player);
    bool update_player_stats(PlayerID id, const PlayerStats &stats);
    bool get_player_stats(PlayerID id, PlayerStats &stats);

    // Match history
    bool record_match_start(uint32_t match_id, const std::vector<PlayerID> &players);
    bool record_match_end(uint32_t match_id, const std::vector<PlayerID> &winners);
    std::vector<MatchRecord> get_player_match_history(PlayerID player_id, size_t limit = 10);

    // Resource tracking
    bool record_resource_transaction(PlayerID player_id, const std::string &resource, int amount);
    int get_total_resources_gathered(PlayerID player_id);

private:
    bool create_tables();
    bool execute_query(const std::string &query);

    sqlite3 *db_{nullptr};
    std::string db_path_;
};