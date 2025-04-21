#include "networking/client_connection.hpp"
#include "networking/message_utils.hpp"
#include <iostream>

using namespace message_utils;

ClientConnection::ClientConnection(tcp::socket socket, PlayerID player_id)
    : socket_(std::move(socket)), player_id_(player_id), read_buffer_(header_length)
{
}

ClientConnection::~ClientConnection() = default;

void ClientConnection::start()
{
    connected_ = true;
    do_read_header();
}

void ClientConnection::stop()
{
    if (connected_)
    {
        connected_ = false;
        boost::system::error_code ec;
        socket_.close(ec);
    }
}

void ClientConnection::send_message(const Message &message)
{
    auto serialized = message.serialize();
    bool write_in_progress = !write_queue_.empty();
    write_queue_.push(std::move(serialized));

    if (!write_in_progress)
    {
        do_write();
    }
}

void ClientConnection::do_read_header()
{
    auto self(shared_from_this());
    boost::asio::async_read(socket_,
                            boost::asio::buffer(read_buffer_, header_length),
                            [this, self](boost::system::error_code ec, std::size_t /*length*/)
                            {
                                if (!ec && connected_)
                                {
                                    message_length_ =
                                        (static_cast<uint32_t>(read_buffer_[0]) << 24) |
                                        (static_cast<uint32_t>(read_buffer_[1]) << 16) |
                                        (static_cast<uint32_t>(read_buffer_[2]) << 8) |
                                        static_cast<uint32_t>(read_buffer_[3]);

                                    message_buffer_.resize(message_length_);
                                    do_read_body();
                                }
                                else
                                {
                                    stop();
                                }
                            });
}

void ClientConnection::do_read_body()
{
    auto self(shared_from_this());
    boost::asio::async_read(socket_,
                            boost::asio::buffer(message_buffer_, message_length_),
                            [this, self](boost::system::error_code ec, std::size_t /*length*/)
                            {
                                if (!ec && connected_)
                                {
                                    Message message = Message::deserialize(message_buffer_);
                                    handle_message(message);
                                    do_read_header();
                                }
                                else
                                {
                                    stop();
                                }
                            });
}

void ClientConnection::do_write()
{
    auto self(shared_from_this());
    boost::asio::async_write(socket_,
                             boost::asio::buffer(write_queue_.front()),
                             [this, self](boost::system::error_code ec, std::size_t /*length*/)
                             {
                                 if (!ec && connected_)
                                 {
                                     write_queue_.pop();
                                     if (!write_queue_.empty())
                                     {
                                         do_write();
                                     }
                                 }
                                 else
                                 {
                                     stop();
                                 }
                             });
}

void ClientConnection::handle_message(const Message &message)
{
    switch (message.type)
    {
    case MessageType::Connect:
        handle_connect(message);
        break;
    case MessageType::Disconnect:
        handle_disconnect();
        break;
    case MessageType::ChatMessage:
        handle_chat_message(message);
        break;
    case MessageType::PlayerMove:
        handle_move_request(message);
        break;
    case MessageType::PlayerBuild:
        handle_build_request(message);
        break;
    case MessageType::PlayerAttack:
        handle_attack_request(message);
        break;
    case MessageType::PlayerHarvest:
        handle_harvest_request(message);
        break;
    case MessageType::RequestUpgrade:
        handle_upgrade_request(message);
        break;
    case MessageType::RequestTechnology:
        handle_technology_request(message);
        break;
    case MessageType::UpgradeListRequest:
        handle_upgrade_list_request();
        break;
    case MessageType::UpgradeResponse:
        handle_upgrade_response(message);
        break;
    case MessageType::TechnologyResponse:
        handle_technology_response(message);
        break;
    case MessageType::UpgradeListResponse:
        handle_upgrade_list_response(message);
        break;
    default:
        std::cerr << "Unknown message type received\n";
        break;
    }

    if (message_handler_)
    {
        message_handler_(message);
    }
}

void ClientConnection::handle_connect(const Message &message)
{
    connected_ = true;
    // Send acknowledgment back to client
    Message response;
    response.type = MessageType::ConnectResponse;
    response.player_id = player_id_;
    send_message(response);
}

void ClientConnection::handle_disconnect()
{
    connected_ = false;
    socket_.close();
}

void ClientConnection::handle_chat_message(const Message &message)
{
    if (!authenticated_)
        return;

    // Forward the chat message to the message handler if set
    if (message_handler_)
    {
        message_handler_(message);
    }
}

void ClientConnection::handle_move_request(const Message &message)
{
    if (!authenticated_)
        return;

    // Forward the move request to the message handler
    if (message_handler_)
    {
        message_handler_(message);
    }
}

void ClientConnection::handle_build_request(const Message &message)
{
    if (!authenticated_)
        return;

    // Forward the build request to the message handler
    if (message_handler_)
    {
        message_handler_(message);
    }
}

void ClientConnection::handle_attack_request(const Message &message)
{
    if (!authenticated_)
        return;

    // Forward the attack request to the message handler
    if (message_handler_)
    {
        message_handler_(message);
    }
}

void ClientConnection::handle_harvest_request(const Message &message)
{
    if (!authenticated_)
        return;

    // Forward the harvest request to the message handler
    if (message_handler_)
    {
        message_handler_(message);
    }
}

void ClientConnection::handle_upgrade_request(const Message &message)
{
    if (!is_authenticated())
    {
        return;
    }

    // Forward the upgrade request to the server through the message handler
    if (message_handler_)
    {
        message_handler_(message);
    }
}

void ClientConnection::handle_technology_request(const Message &message)
{
    if (!is_authenticated())
    {
        return;
    }

    // Forward the technology request to the server through the message handler
    if (message_handler_)
    {
        message_handler_(message);
    }
}

void ClientConnection::handle_upgrade_list_request()
{
    if (!is_authenticated())
    {
        return;
    }

    // Create and forward the upgrade list request
    Message request;
    request.type = MessageType::UpgradeListRequest;

    if (message_handler_)
    {
        message_handler_(request);
    }
}

void ClientConnection::handle_upgrade_response(const Message &message)
{
    if (!is_authenticated())
    {
        return;
    }

    size_t offset = 0;
    bool success = message_utils::read_from_vector<bool>(message.data, offset);
    std::string upgrade_name = message_utils::read_string(message.data, offset);
    int new_level = message_utils::read_from_vector<int>(message.data, offset);

    std::cout << "Upgrade " << upgrade_name << " "
              << (success ? "succeeded" : "failed")
              << " (new level: " << new_level << ")\n";
}

void ClientConnection::handle_technology_response(const Message &message)
{
    if (!is_authenticated())
    {
        return;
    }

    size_t offset = 0;
    bool success = message_utils::read_from_vector<bool>(message.data, offset);
    std::string tech_name = message_utils::read_string(message.data, offset);

    std::cout << "Technology " << tech_name << " "
              << (success ? "unlocked" : "failed to unlock") << "\n";
}

void ClientConnection::handle_upgrade_list_response(const Message &message)
{
    if (!is_authenticated())
    {
        return;
    }

    size_t offset = 0;

    // Read available upgrades
    uint32_t upgrade_count = message_utils::read_from_vector<uint32_t>(message.data, offset);
    std::vector<std::string> available_upgrades;
    for (uint32_t i = 0; i < upgrade_count; ++i)
    {
        available_upgrades.push_back(message_utils::read_string(message.data, offset));
    }

    // Read available technologies
    uint32_t tech_count = message_utils::read_from_vector<uint32_t>(message.data, offset);
    std::vector<std::string> available_technologies;
    for (uint32_t i = 0; i < tech_count; ++i)
    {
        available_technologies.push_back(message_utils::read_string(message.data, offset));
    }

    std::cout << "Received upgrade list update:\n";
    std::cout << "Available upgrades: " << upgrade_count << "\n";
    std::cout << "Available technologies: " << tech_count << "\n";
}