#include "server/chat_handler.hpp"
#include <chrono>
#include <algorithm>

ChatHandler::ChatHandler(size_t max_history)
    : max_history_(max_history)
{
}

ChatHandler::~ChatHandler() = default;

void ChatHandler::broadcast_message(PlayerID sender_id, const std::string &message, bool team_only)
{
    if (is_player_muted(sender_id))
    {
        return;
    }

    ChatMessage chat_message{
        sender_id,
        message,
        team_only,
        std::chrono::system_clock::now().time_since_epoch().count()};

    message_history_.push_back(chat_message);

    while (message_history_.size() > max_history_)
    {
        message_history_.pop_front();
    }
}

std::vector<ChatMessage> ChatHandler::get_recent_messages(size_t count) const
{
    count = std::min(count, message_history_.size());
    std::vector<ChatMessage> recent_messages;
    recent_messages.reserve(count);

    auto start = message_history_.end() - count;
    recent_messages.assign(start, message_history_.end());
    return recent_messages;
}

void ChatHandler::clear_history()
{
    message_history_.clear();
}

bool ChatHandler::is_player_muted(PlayerID player_id) const
{
    return std::find(muted_players_.begin(), muted_players_.end(), player_id) != muted_players_.end();
}

void ChatHandler::mute_player(PlayerID player_id)
{
    if (!is_player_muted(player_id))
    {
        muted_players_.push_back(player_id);
    }
}

void ChatHandler::unmute_player(PlayerID player_id)
{
    auto it = std::find(muted_players_.begin(), muted_players_.end(), player_id);
    if (it != muted_players_.end())
    {
        muted_players_.erase(it);
    }
}