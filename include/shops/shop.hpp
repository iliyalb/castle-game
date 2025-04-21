#pragma once

#include <string>
#include <vector>
#include "../utils/types.hpp"
#include "../items/item.hpp"
#include "../units/unit.hpp"
#include "../server/player.hpp"

struct ItemPrice
{
    std::string resource_type;
    int amount;
};

struct ShopItem
{
    std::string name;
    std::string description;
    std::vector<ItemPrice> prices;
    bool is_available;
    int stock; // -1 for unlimited
};

class Shop
{
public:
    Shop(const std::string &name, const std::string &description);
    virtual ~Shop() = default;

    // Basic shop operations
    bool can_afford(PlayerID player_id, const ShopItem &item) const;
    bool purchase_item(PlayerID player_id, const std::string &item_name);
    bool sell_item(PlayerID player_id, const std::string &item_name);

    // Shop management
    void add_item(const ShopItem &item);
    void remove_item(const std::string &item_name);
    void update_stock(const std::string &item_name, int new_stock);
    void set_item_availability(const std::string &item_name, bool available);

    // Getters
    const std::string &get_name() const { return name_; }
    const std::string &get_description() const { return description_; }
    std::vector<ShopItem> get_available_items() const;
    const ShopItem *get_item(const std::string &item_name) const;

protected:
    std::string name_;
    std::string description_;
    std::map<std::string, ShopItem> items_;
};