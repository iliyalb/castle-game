#include "shops/shop.hpp"
#include "server/resource_manager.hpp"
#include <algorithm>

Shop::Shop(const std::string &name, const std::string &description)
    : name_(name), description_(description)
{
}

bool Shop::can_afford(PlayerID player_id, const ShopItem &item) const
{
    // TODO: Integrate with ResourceManager to check player resources
    // For now, assume the player can afford it
    return true;
}

bool Shop::purchase_item(PlayerID player_id, const std::string &item_name)
{
    auto it = items_.find(item_name);
    if (it == items_.end() || !it->second.is_available)
    {
        return false;
    }

    const ShopItem &item = it->second;
    if (item.stock == 0)
    {
        return false;
    }

    if (!can_afford(player_id, item))
    {
        return false;
    }

    // Deduct resources and update stock
    if (item.stock > 0)
    {
        items_[item_name].stock--;
    }

    return true;
}

bool Shop::sell_item(PlayerID player_id, const std::string &item_name)
{
    auto it = items_.find(item_name);
    if (it == items_.end())
    {
        return false;
    }

    // Add half the purchase price back to player's resources
    // TODO: Integrate with ResourceManager to add resources back

    // Update stock if limited
    if (it->second.stock >= 0)
    {
        items_[item_name].stock++;
    }

    return true;
}

void Shop::add_item(const ShopItem &item)
{
    items_[item.name] = item;
}

void Shop::remove_item(const std::string &item_name)
{
    items_.erase(item_name);
}

void Shop::update_stock(const std::string &item_name, int new_stock)
{
    auto it = items_.find(item_name);
    if (it != items_.end())
    {
        it->second.stock = new_stock;
    }
}

void Shop::set_item_availability(const std::string &item_name, bool available)
{
    auto it = items_.find(item_name);
    if (it != items_.end())
    {
        it->second.is_available = available;
    }
}

std::vector<ShopItem> Shop::get_available_items() const
{
    std::vector<ShopItem> available;
    for (const auto &[name, item] : items_)
    {
        if (item.is_available && (item.stock != 0))
        {
            available.push_back(item);
        }
    }
    return available;
}

const ShopItem *Shop::get_item(const std::string &item_name) const
{
    auto it = items_.find(item_name);
    return it != items_.end() ? &it->second : nullptr;
}