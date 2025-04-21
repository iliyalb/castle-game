#pragma once

#include <vector>
#include <string>
#include <cstring>
#include <cstdint>

namespace message_utils
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

    inline void write_string(std::vector<uint8_t> &vec, const std::string &str)
    {
        uint32_t length = static_cast<uint32_t>(str.length());
        write_to_vector(vec, length);
        vec.insert(vec.end(), str.begin(), str.end());
    }

    inline std::string read_string(const std::vector<uint8_t> &vec, size_t &offset)
    {
        uint32_t length = read_from_vector<uint32_t>(vec, offset);
        std::string str(vec.begin() + offset, vec.begin() + offset + length);
        offset += length;
        return str;
    }
}