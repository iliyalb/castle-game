#include "server/castle_server.hpp"
#include "server/player_manager.hpp"
#include "networking/client_connection.hpp"
#include "networking/message_utils.hpp"
#include <iostream>

using namespace message_utils;

CastleServer::CastleServer(boost::asio::io_context &io_context, unsigned short port)
    : io_context_(io_context), acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
{
    game_state_ = std::make_unique<GameState>();
    player_manager_ = std::make_unique<PlayerManager>();
    map_ = std::make_unique<Map>(100, 100, 32); // Default 100x100 map with 32px tiles
    resource_manager_ = std::make_unique<ResourceManager>();
    chat_handler_ = std::make_unique<ChatHandler>();
    timer_ = std::make_unique<Timer>();
}

CastleServer::~CastleServer()
{
    stop();
}

void CastleServer::start()
{
    if (!running_)
    {
        running_ = true;
        accept_connections();
    }
}

void CastleServer::stop()
{
    if (running_)
    {
        running_ = false;
        acceptor_.close();

        for (auto &client : clients_)
        {
            if (client)
            {
                client->stop();
            }
        }
        clients_.clear();
    }
}

void CastleServer::accept_connections()
{
    acceptor_.async_accept(
        [this](std::error_code ec, tcp::socket socket)
        {
            if (!ec)
            {
                auto connection = handle_client(std::move(socket));
                if (connection)
                {
                    clients_.push_back(connection);
                }
            }

            if (running_)
            {
                accept_connections();
            }
        });
}

std::shared_ptr<ClientConnection> CastleServer::handle_client(tcp::socket socket)
{
    try
    {
        auto client = std::make_shared<ClientConnection>(
            std::move(socket),
            player_manager_->generate_next_player_id());
        client->start();
        return client;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error handling client connection: " << e.what() << std::endl;
        return nullptr;
    }
}

void CastleServer::handle_upgrade_request(PlayerID player_id, const Message &message)
{
    size_t offset = 0;
    std::string upgrade_name = message_utils::read_string(message.data, offset);

    auto &upgrade_manager = game_state_->get_upgrade_manager();
    bool success = upgrade_manager.purchase_upgrade(player_id, upgrade_name);
    int new_level = upgrade_manager.get_upgrade_level(player_id, upgrade_name);

    send_upgrade_response(player_id, success, upgrade_name, new_level);
}

void CastleServer::handle_technology_request(PlayerID player_id, const Message &message)
{
    size_t offset = 0;
    std::string tech_name = message_utils::read_string(message.data, offset);

    auto &upgrade_manager = game_state_->get_upgrade_manager();
    bool success = upgrade_manager.unlock_technology(player_id, tech_name);

    send_technology_response(player_id, success, tech_name);
}

void CastleServer::handle_upgrade_list_request(PlayerID player_id)
{
    auto &upgrade_manager = game_state_->get_upgrade_manager();
    auto available_upgrades = upgrade_manager.get_available_upgrades(player_id);
    auto available_technologies = upgrade_manager.get_available_technologies(player_id);

    Message response = Message::create_upgrade_list_response(
        available_upgrades, available_technologies);

    auto connection_it = connections_.find(player_id);
    if (connection_it != connections_.end())
    {
        connection_it->second->send_message(response);
    }
}

void CastleServer::send_upgrade_response(PlayerID player_id, bool success,
                                         const std::string &upgrade_name, int new_level)
{
    Message response = Message::create_upgrade_response(success, upgrade_name, new_level);

    auto connection_it = connections_.find(player_id);
    if (connection_it != connections_.end())
    {
        connection_it->second->send_message(response);
    }
}

void CastleServer::send_technology_response(PlayerID player_id, bool success,
                                            const std::string &tech_name)
{
    Message response = Message::create_technology_response(success, tech_name);

    auto connection_it = connections_.find(player_id);
    if (connection_it != connections_.end())
    {
        connection_it->second->send_message(response);
    }
}