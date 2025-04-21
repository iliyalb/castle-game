#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <queue>
#include <vector>
#include <functional>
#include "../server/game_state.hpp"
#include "../server/player.hpp"
#include "../utils/types.hpp"
#include "message.hpp"

using boost::asio::ip::tcp;
using MessageHandler = std::function<void(const Message &)>;

class ClientConnection : public std::enable_shared_from_this<ClientConnection>
{
public:
    explicit ClientConnection(tcp::socket socket, PlayerID player_id);
    ~ClientConnection();

    void start();
    void stop();
    void send_message(const Message &message);
    PlayerID get_player_id() const { return player_id_; }
    bool is_connected() const { return connected_; }
    void set_authenticated(bool auth) { authenticated_ = auth; }
    bool is_authenticated() const { return authenticated_; }

    void set_message_handler(MessageHandler handler) { message_handler_ = std::move(handler); }

private:
    void do_read_header();
    void do_read_body();
    void do_write();
    void handle_message(const Message &message);

    // Message handlers
    void handle_connect(const Message &message);
    void handle_disconnect();
    void handle_chat_message(const Message &message);
    void handle_move_request(const Message &message);
    void handle_build_request(const Message &message);
    void handle_attack_request(const Message &message);
    void handle_harvest_request(const Message &message);

    // Upgrade message handlers
    void handle_upgrade_request(const Message &message);
    void handle_technology_request(const Message &message);
    void handle_upgrade_list_request();
    void handle_upgrade_response(const Message &message);
    void handle_technology_response(const Message &message);
    void handle_upgrade_list_response(const Message &message);

    tcp::socket socket_;
    PlayerID player_id_;
    bool connected_{false};
    bool authenticated_{false};
    MessageHandler message_handler_;

    enum
    {
        header_length = 4
    };
    std::vector<uint8_t> read_buffer_;
    std::queue<std::vector<uint8_t>> write_queue_;
    Message current_read_message_;
    uint32_t current_message_size_{0};
    uint32_t message_length_{0}; // Added missing member
    bool writing_{false};
    std::vector<uint8_t> message_buffer_;
};