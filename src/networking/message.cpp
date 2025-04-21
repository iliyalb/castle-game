#include "networking/message.hpp"
#include <cstring>

namespace
{
    template <typename T>
    void write_to_vector(std::vector<uint8_t> &vec, const T &value)
    {
        const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&value);
        vec.insert(vec.end(), bytes, bytes + sizeof(T));
    }

    template <typename T>
    T read_from_vector(const std::vector<uint8_t> &vec, size_t &offset)
    {
        T value;
        std::memcpy(&value, vec.data() + offset, sizeof(T));
        offset += sizeof(T);
        return value;
    }

    void write_string(std::vector<uint8_t> &vec, const std::string &str)
    {
        uint32_t length = static_cast<uint32_t>(str.length());
        write_to_vector(vec, length);
        vec.insert(vec.end(), str.begin(), str.end());
    }

    std::string read_string(const std::vector<uint8_t> &vec, size_t &offset)
    {
        uint32_t length = read_from_vector<uint32_t>(vec, offset);
        std::string str(vec.begin() + offset, vec.begin() + offset + length);
        offset += length;
        return str;
    }
}

Message Message::create_connect(const std::string &player_name)
{
    Message msg;
    msg.type = MessageType::Connect;
    write_string(msg.data, player_name);
    return msg;
}

Message Message::create_chat(const std::string &text, bool team_only)
{
    Message msg;
    msg.type = MessageType::ChatMessage;
    write_to_vector(msg.data, team_only);
    write_string(msg.data, text);
    return msg;
}

Message Message::create_move(int x, int y, std::vector<uint32_t> unit_ids)
{
    Message msg;
    msg.type = MessageType::PlayerMove;
    write_to_vector(msg.data, x);
    write_to_vector(msg.data, y);
    uint32_t count = static_cast<uint32_t>(unit_ids.size());
    write_to_vector(msg.data, count);
    for (const auto &id : unit_ids)
    {
        write_to_vector(msg.data, id);
    }
    return msg;
}

Message Message::create_build(int x, int y, uint32_t building_type)
{
    Message msg;
    msg.type = MessageType::PlayerBuild;
    write_to_vector(msg.data, x);
    write_to_vector(msg.data, y);
    write_to_vector(msg.data, building_type);
    return msg;
}

Message Message::create_attack(uint32_t attacker_id, uint32_t target_id)
{
    Message msg;
    msg.type = MessageType::PlayerAttack;
    write_to_vector(msg.data, attacker_id);
    write_to_vector(msg.data, target_id);
    return msg;
}

Message Message::create_harvest(uint32_t unit_id, uint32_t resource_id)
{
    Message msg;
    msg.type = MessageType::PlayerHarvest;
    write_to_vector(msg.data, unit_id);
    write_to_vector(msg.data, resource_id);
    return msg;
}

Message Message::create_upgrade_request(const std::string &upgrade_name)
{
    Message msg;
    msg.type = MessageType::RequestUpgrade;
    write_string(msg.data, upgrade_name);
    return msg;
}

Message Message::create_upgrade_response(bool success, const std::string &upgrade_name, int new_level)
{
    Message msg;
    msg.type = MessageType::UpgradeResponse;
    write_to_vector(msg.data, success);
    write_string(msg.data, upgrade_name);
    write_to_vector(msg.data, new_level);
    return msg;
}

Message Message::create_technology_request(const std::string &tech_name)
{
    Message msg;
    msg.type = MessageType::RequestTechnology;
    write_string(msg.data, tech_name);
    return msg;
}

Message Message::create_technology_response(bool success, const std::string &tech_name)
{
    Message msg;
    msg.type = MessageType::TechnologyResponse;
    write_to_vector(msg.data, success);
    write_string(msg.data, tech_name);
    return msg;
}

Message Message::create_upgrade_list_request()
{
    Message msg;
    msg.type = MessageType::UpgradeListRequest;
    return msg;
}

Message Message::create_upgrade_list_response(
    const std::vector<std::string> &available_upgrades,
    const std::vector<std::string> &available_technologies)
{
    Message msg;
    msg.type = MessageType::UpgradeListResponse;

    // Write upgrades
    uint32_t upgrade_count = static_cast<uint32_t>(available_upgrades.size());
    write_to_vector(msg.data, upgrade_count);
    for (const auto &upgrade : available_upgrades)
    {
        write_string(msg.data, upgrade);
    }

    // Write technologies
    uint32_t tech_count = static_cast<uint32_t>(available_technologies.size());
    write_to_vector(msg.data, tech_count);
    for (const auto &tech : available_technologies)
    {
        write_string(msg.data, tech);
    }

    return msg;
}

std::vector<uint8_t> Message::serialize() const
{
    std::vector<uint8_t> result;
    write_to_vector(result, type);
    uint32_t size = static_cast<uint32_t>(data.size());
    write_to_vector(result, size);
    result.insert(result.end(), data.begin(), data.end());
    return result;
}

Message Message::deserialize(const std::vector<uint8_t> &data)
{
    Message msg;
    size_t offset = 0;
    msg.type = read_from_vector<MessageType>(data, offset);
    uint32_t size = read_from_vector<uint32_t>(data, offset);
    msg.data.assign(data.begin() + offset, data.begin() + offset + size);
    return msg;
}