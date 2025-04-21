#include "shops/specific_shops.hpp"
#include <algorithm>
#include <memory>
#include <random>

// Armory Shop Implementation
ArmoryShop::ArmoryShop()
    : Shop("Armory", "Military equipment and armor shop")
{
    initialize_weapons();
    initialize_armor();
}

void ArmoryShop::initialize_weapons()
{
    add_item({"Iron Sword",
              "Basic melee weapon",
              {{"gold", 100}, {"iron", 20}},
              true,
              -1});

    add_item({"Steel Sword",
              "Advanced melee weapon",
              {{"gold", 200}, {"iron", 30}, {"coal", 10}},
              true,
              -1});

    add_item({"Longbow",
              "Advanced ranged weapon",
              {{"gold", 150}, {"wood", 30}},
              true,
              -1});
}

void ArmoryShop::initialize_armor()
{
    add_item({"Leather Armor",
              "Basic protective gear",
              {{"gold", 80}, {"leather", 20}},
              true,
              -1});

    add_item({"Chain Mail",
              "Medium armor",
              {{"gold", 200}, {"iron", 40}},
              true,
              -1});
}

// Unit Training Shop Implementation
UnitTrainingShop::UnitTrainingShop()
    : Shop("Training Grounds", "Train new military units")
{
    initialize_trainable_units();
}

void UnitTrainingShop::initialize_trainable_units()
{
    add_item({"Train Soldier",
              "Basic military unit",
              {{"gold", 100}, {"food", 50}},
              true,
              -1});

    add_item({"Train Archer",
              "Ranged combat unit",
              {{"gold", 120}, {"food", 50}, {"wood", 30}},
              true,
              -1});

    add_item({"Train Knight",
              "Heavy cavalry unit",
              {{"gold", 200}, {"food", 100}, {"iron", 50}},
              true,
              -1});
}

std::unique_ptr<Unit> UnitTrainingShop::train_unit(PlayerID player_id, UnitType type)
{
    std::string item_name;
    switch (type)
    {
    case UnitType::Soldier:
        item_name = "Train Soldier";
        break;
    case UnitType::Archer:
        item_name = "Train Archer";
        break;
    case UnitType::Knight:
        item_name = "Train Knight";
        break;
    default:
        return nullptr;
    }

    if (purchase_item(player_id, item_name))
    {
        switch (type)
        {
        case UnitType::Soldier:
            return std::make_unique<Soldier>(player_id);
        case UnitType::Archer:
            return std::make_unique<Archer>(player_id);
        case UnitType::Knight:
            return std::make_unique<Knight>(player_id);
        default:
            return nullptr;
        }
    }
    return nullptr;
}

// Resource Shop Implementation
ResourceShop::ResourceShop()
    : Shop("Resource Exchange", "Trade basic resources")
{
    initialize_resources();
}

void ResourceShop::initialize_resources()
{
    add_item({"Wood Bundle",
              "Basic building material",
              {{"gold", 50}},
              true,
              -1});

    add_item({"Iron Ore",
              "Basic crafting material",
              {{"gold", 75}},
              true,
              -1});

    add_item({"Food Supply",
              "Basic sustenance",
              {{"gold", 40}},
              true,
              -1});
}

// Marketplace Shop Implementation
MarketplaceShop::MarketplaceShop()
    : Shop("Marketplace", "Dynamic trading post with fluctuating prices")
{
    initialize_trade_goods();
    update_prices();
}

void MarketplaceShop::initialize_trade_goods()
{
    add_item({"Luxury Goods",
              "High-value trade items",
              {{"gold", 300}},
              true,
              10});

    add_item({"Spices",
              "Valuable trade goods",
              {{"gold", 200}},
              true,
              15});

    add_item({"Textiles",
              "Cloth and fabric goods",
              {{"gold", 150}},
              true,
              20});

    // Initialize price multipliers
    price_multipliers_ = {
        {"Luxury Goods", 1.0f},
        {"Spices", 1.0f},
        {"Textiles", 1.0f}};
}

void MarketplaceShop::update_prices()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.8f, 1.2f);

    for (auto &[item_name, multiplier] : price_multipliers_)
    {
        multiplier = dis(gen);
        auto item = get_item(item_name);
        if (item)
        {
            // Update prices based on new multiplier
            for (auto &price : items_[item_name].prices)
            {
                price.amount = static_cast<int>(price.amount * multiplier);
            }
        }
    }
}