#pragma once

#include "shop.hpp"
#include "../units/specific_units.hpp"
#include <memory>
#include <vector>

class ArmoryShop : public Shop
{
public:
    ArmoryShop();

private:
    void initialize_weapons();
    void initialize_armor();
};

class UnitTrainingShop : public Shop
{
public:
    UnitTrainingShop();
    std::unique_ptr<Unit> train_unit(PlayerID player_id, UnitType type);

private:
    void initialize_trainable_units();
};

class ResourceShop : public Shop
{
public:
    ResourceShop();

private:
    void initialize_resources();
};

class MarketplaceShop : public Shop
{
public:
    MarketplaceShop();
    void update_prices(); // Updates prices based on supply/demand
private:
    void initialize_trade_goods();
    std::map<std::string, float> price_multipliers_;
};