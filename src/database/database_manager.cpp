#include "database/database_manager.hpp"
#include <sstream>

DatabaseManager::DatabaseManager(const std::string &db_path)
    : db_path_(db_path)
{
}

DatabaseManager::~DatabaseManager()
{
    if (db_)
    {
        sqlite3_close(db_);
    }
}

bool DatabaseManager::initialize()
{
    int rc = sqlite3_open(db_path_.c_str(), &db_);
    if (rc)
    {
        return false;
    }
    return create_tables();
}

bool DatabaseManager::create_tables()
{
    const char *queries[] = {
        // Players table
        "CREATE TABLE IF NOT EXISTS players ("
        "    id INTEGER PRIMARY KEY,"
        "    name TEXT NOT NULL,"
        "    faction INTEGER NOT NULL,"
        "    color_r INTEGER NOT NULL,"
        "    color_g INTEGER NOT NULL,"
        "    color_b INTEGER NOT NULL,"
        "    color_a INTEGER NOT NULL"
        ");",

        // Player stats table
        "CREATE TABLE IF NOT EXISTS player_stats ("
        "    player_id INTEGER PRIMARY KEY,"
        "    matches_played INTEGER DEFAULT 0,"
        "    matches_won INTEGER DEFAULT 0,"
        "    total_resources_gathered INTEGER DEFAULT 0,"
        "    total_units_created INTEGER DEFAULT 0,"
        "    total_buildings_constructed INTEGER DEFAULT 0,"
        "    FOREIGN KEY(player_id) REFERENCES players(id)"
        ");",

        // Matches table
        "CREATE TABLE IF NOT EXISTS matches ("
        "    id INTEGER PRIMARY KEY,"
        "    start_time INTEGER NOT NULL,"
        "    end_time INTEGER,"
        "    status TEXT NOT NULL"
        ");",

        // Match players table
        "CREATE TABLE IF NOT EXISTS match_players ("
        "    match_id INTEGER,"
        "    player_id INTEGER,"
        "    is_winner BOOLEAN,"
        "    PRIMARY KEY(match_id, player_id),"
        "    FOREIGN KEY(match_id) REFERENCES matches(id),"
        "    FOREIGN KEY(player_id) REFERENCES players(id)"
        ");",

        // Resource transactions table
        "CREATE TABLE IF NOT EXISTS resource_transactions ("
        "    id INTEGER PRIMARY KEY,"
        "    player_id INTEGER,"
        "    resource_type TEXT NOT NULL,"
        "    amount INTEGER NOT NULL,"
        "    timestamp INTEGER NOT NULL,"
        "    FOREIGN KEY(player_id) REFERENCES players(id)"
        ");"};

    for (const auto &query : queries)
    {
        if (!execute_query(query))
        {
            return false;
        }
    }
    return true;
}

bool DatabaseManager::execute_query(const std::string &query)
{
    char *err_msg = nullptr;
    int rc = sqlite3_exec(db_, query.c_str(), nullptr, nullptr, &err_msg);

    if (rc != SQLITE_OK)
    {
        sqlite3_free(err_msg);
        return false;
    }
    return true;
}

bool DatabaseManager::save_player(const Player &player)
{
    std::stringstream ss;
    ss << "INSERT OR REPLACE INTO players (id, name, faction, color_r, color_g, color_b, color_a) "
       << "VALUES (" << player.get_id() << ", "
       << "'" << player.get_name() << "', "
       << static_cast<int>(player.get_faction()) << ", "
       << static_cast<int>(player.get_color().r) << ", "
       << static_cast<int>(player.get_color().g) << ", "
       << static_cast<int>(player.get_color().b) << ", "
       << static_cast<int>(player.get_color().a) << ");";

    return execute_query(ss.str());
}

bool DatabaseManager::load_player(PlayerID id, Player &player)
{
    std::stringstream ss;
    ss << "SELECT name, faction, color_r, color_g, color_b, color_a "
       << "FROM players WHERE id = " << id << ";";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db_, ss.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    bool success = false;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char *name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        int faction = sqlite3_column_int(stmt, 1);
        Color color{
            static_cast<uint8_t>(sqlite3_column_int(stmt, 2)),
            static_cast<uint8_t>(sqlite3_column_int(stmt, 3)),
            static_cast<uint8_t>(sqlite3_column_int(stmt, 4)),
            static_cast<uint8_t>(sqlite3_column_int(stmt, 5))};

        player = Player(id, static_cast<FactionType>(faction), color, 1.0f);
        player.set_name(name);
        success = true;
    }

    sqlite3_finalize(stmt);
    return success;
}

bool DatabaseManager::update_player_stats(PlayerID id, const PlayerStats &stats)
{
    std::stringstream ss;
    ss << "INSERT OR REPLACE INTO player_stats "
       << "(player_id, matches_played, matches_won, total_resources_gathered, "
       << "total_units_created, total_buildings_constructed) VALUES ("
       << stats.player_id << ", "
       << stats.matches_played << ", "
       << stats.matches_won << ", "
       << stats.total_resources_gathered << ", "
       << stats.total_units_created << ", "
       << stats.total_buildings_constructed << ");";

    return execute_query(ss.str());
}

bool DatabaseManager::get_player_stats(PlayerID id, PlayerStats &stats)
{
    std::stringstream ss;
    ss << "SELECT matches_played, matches_won, total_resources_gathered, "
       << "total_units_created, total_buildings_constructed "
       << "FROM player_stats WHERE player_id = " << id << ";";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db_, ss.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    bool success = false;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        stats.player_id = id;
        stats.matches_played = sqlite3_column_int(stmt, 0);
        stats.matches_won = sqlite3_column_int(stmt, 1);
        stats.total_resources_gathered = sqlite3_column_int(stmt, 2);
        stats.total_units_created = sqlite3_column_int(stmt, 3);
        stats.total_buildings_constructed = sqlite3_column_int(stmt, 4);
        success = true;
    }

    sqlite3_finalize(stmt);
    return success;
}

bool DatabaseManager::record_match_start(uint32_t match_id, const std::vector<PlayerID> &players)
{
    std::stringstream ss;
    ss << "INSERT INTO matches (id, start_time, status) VALUES ("
       << match_id << ", "
       << "strftime('%s', 'now'), "
       << "'in_progress');";

    if (!execute_query(ss.str()))
    {
        return false;
    }

    for (const auto &player_id : players)
    {
        ss.str("");
        ss << "INSERT INTO match_players (match_id, player_id, is_winner) VALUES ("
           << match_id << ", " << player_id << ", 0);";
        if (!execute_query(ss.str()))
        {
            return false;
        }
    }
    return true;
}

bool DatabaseManager::record_match_end(uint32_t match_id, const std::vector<PlayerID> &winners)
{
    std::stringstream ss;
    ss << "UPDATE matches SET "
       << "end_time = strftime('%s', 'now'), "
       << "status = 'completed' "
       << "WHERE id = " << match_id << ";";

    if (!execute_query(ss.str()))
    {
        return false;
    }

    for (const auto &winner_id : winners)
    {
        ss.str("");
        ss << "UPDATE match_players SET is_winner = 1 "
           << "WHERE match_id = " << match_id
           << " AND player_id = " << winner_id << ";";
        if (!execute_query(ss.str()))
        {
            return false;
        }
    }
    return true;
}

std::vector<MatchRecord> DatabaseManager::get_player_match_history(PlayerID player_id, size_t limit)
{
    std::stringstream ss;
    ss << "SELECT m.id, m.start_time, m.end_time, mp.is_winner, mp2.player_id "
       << "FROM matches m "
       << "JOIN match_players mp ON m.id = mp.match_id "
       << "JOIN match_players mp2 ON m.id = mp2.match_id "
       << "WHERE mp.player_id = " << player_id
       << " AND m.status = 'completed' "
       << "ORDER BY m.end_time DESC LIMIT " << limit << ";";

    std::vector<MatchRecord> records;
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db_, ss.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        return records;
    }

    MatchRecord current_record{};
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        uint32_t match_id = sqlite3_column_int(stmt, 0);

        if (current_record.match_id != match_id)
        {
            if (current_record.match_id != 0)
            {
                records.push_back(current_record);
            }
            current_record = MatchRecord{};
            current_record.match_id = match_id;
            current_record.start_time = sqlite3_column_int64(stmt, 1);
            current_record.end_time = sqlite3_column_int64(stmt, 2);
        }

        bool is_winner = sqlite3_column_int(stmt, 3) != 0;
        PlayerID other_player_id = sqlite3_column_int(stmt, 4);

        if (is_winner)
        {
            current_record.winners.push_back(other_player_id);
        }
        else
        {
            current_record.losers.push_back(other_player_id);
        }
    }

    if (current_record.match_id != 0)
    {
        records.push_back(current_record);
    }

    sqlite3_finalize(stmt);
    return records;
}

bool DatabaseManager::record_resource_transaction(PlayerID player_id, const std::string &resource, int amount)
{
    std::stringstream ss;
    ss << "INSERT INTO resource_transactions "
       << "(player_id, resource_type, amount, timestamp) VALUES ("
       << player_id << ", "
       << "'" << resource << "', "
       << amount << ", "
       << "strftime('%s', 'now'));";

    return execute_query(ss.str());
}

int DatabaseManager::get_total_resources_gathered(PlayerID player_id)
{
    std::stringstream ss;
    ss << "SELECT SUM(amount) FROM resource_transactions "
       << "WHERE player_id = " << player_id
       << " AND amount > 0;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db_, ss.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        return 0;
    }

    int total = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        total = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return total;
}