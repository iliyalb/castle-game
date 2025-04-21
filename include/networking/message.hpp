#pragma once

#include <cstdint>
#include <vector>
#include <string>

enum class MessageType : uint8_t
{
    // Connection messages
    Connect,
    ConnectResponse,
    Disconnect,

    // Game state messages
    JoinGame,
    LeaveGame,
    GameStart,
    GameEnd,

    // Player actions
    PlayerMove,
    PlayerBuild,
    PlayerAttack,
    PlayerHarvest,

    // Resource messages
    ResourceUpdate,

    // Chat messages
    ChatMessage,

    // Upgrade messages
    RequestUpgrade,
    UpgradeResponse,
    RequestTechnology,
    TechnologyResponse,
    UpgradeListRequest,
    UpgradeListResponse,

    // Error messages
    Error
};

struct Message
{
    MessageType type;
    std::vector<uint8_t> data;
    uint32_t player_id{0}; // Added player_id field

    // Basic message creation
    static Message create_connect(const std::string &player_name);
    static Message create_chat(const std::string &text, bool team_only);
    static Message create_move(int x, int y, std::vector<uint32_t> unit_ids);
    static Message create_build(int x, int y, uint32_t building_type);
    static Message create_attack(uint32_t attacker_id, uint32_t target_id);
    static Message create_harvest(uint32_t unit_id, uint32_t resource_id);

    // Upgrade message creation
    static Message create_upgrade_request(const std::string &upgrade_name);
    static Message create_upgrade_response(bool success, const std::string &upgrade_name, int new_level);
    static Message create_technology_request(const std::string &tech_name);
    static Message create_technology_response(bool success, const std::string &tech_name);
    static Message create_upgrade_list_request();
    static Message create_upgrade_list_response(const std::vector<std::string> &available_upgrades,
                                                const std::vector<std::string> &available_technologies);

    std::vector<uint8_t> serialize() const;
    static Message deserialize(const std::vector<uint8_t> &data);
};