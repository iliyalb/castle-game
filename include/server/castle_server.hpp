#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <vector>
#include <map>
#include "../utils/types.hpp"
#include "game_state.hpp"
#include "player_manager.hpp"
#include "map.hpp"
#include "resource_manager.hpp"
#include "chat_handler.hpp"
#include "timer.hpp"
#include "../networking/client_connection.hpp"

using boost::asio::ip::tcp;

class CastleServer
{
public:
    CastleServer(boost::asio::io_context &io_context, unsigned short port);
    ~CastleServer();

    void start();
    void stop();
    std::shared_ptr<ClientConnection> handle_client(tcp::socket socket);
    GameState *get_game_state() { return game_state_.get(); }
    void set_cheat_enabled(bool enabled) { cheat_enabled_ = enabled; }
    void set_game_speed(float speed) { game_speed_ = speed; }

    // Upgrade system handlers
    void handle_upgrade_request(PlayerID player_id, const Message &message);
    void handle_technology_request(PlayerID player_id, const Message &message);
    void handle_upgrade_list_request(PlayerID player_id);
    void send_upgrade_response(PlayerID player_id, bool success,
                               const std::string &upgrade_name, int new_level);
    void send_technology_response(PlayerID player_id, bool success,
                                  const std::string &tech_name);

private:
    void accept_connections();
    void handle_accept(std::error_code ec, tcp::socket socket);

    boost::asio::io_context &io_context_;
    tcp::acceptor acceptor_;
    std::unique_ptr<GameState> game_state_;
    std::unique_ptr<PlayerManager> player_manager_;
    std::unique_ptr<Map> map_;
    std::unique_ptr<ResourceManager> resource_manager_;
    std::unique_ptr<ChatHandler> chat_handler_;
    std::unique_ptr<Timer> timer_;
    bool cheat_enabled_{false};
    float game_speed_{1.0f};
    bool running_{false};
    std::vector<std::shared_ptr<ClientConnection>> clients_;
    std::map<PlayerID, std::shared_ptr<ClientConnection>> connections_;
};