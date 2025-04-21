#pragma once

#include <string>
#include <vector>
#include <deque>
#include <map>
#include "../utils/types.hpp"
#include "game_state.hpp"

struct ChatMessage
{
    PlayerID sender_id;
    std::string message;
    bool is_team_only;
    std::int64_t timestamp;
};

class ChatHandler
{
public:
    ChatHandler(size_t max_history = 100);
    ~ChatHandler();

    void broadcast_message(PlayerID sender_id, const std::string &message, bool team_only = false);
    std::vector<ChatMessage> get_recent_messages(size_t count = 10) const;
    void clear_history();
    bool is_player_muted(PlayerID player_id) const;
    void mute_player(PlayerID player_id);
    void unmute_player(PlayerID player_id);

private:
    std::deque<ChatMessage> message_history_;
    std::vector<PlayerID> muted_players_;
    size_t max_history_;
};