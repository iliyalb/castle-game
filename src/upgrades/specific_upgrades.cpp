#include "upgrades/specific_upgrades.hpp"

WeaponUpgrade::WeaponUpgrade()
    : Upgrade("Weapon Upgrade", "Improves unit attack damage")
{
    this->set_level_scaling(1.2f); // 20% increase per level
    this->add_effect(UpgradeEffect{"attack_damage", 2.0f, true});
    this->add_effect(UpgradeEffect{"attack_speed", 0.1f, true});
}

ArmorUpgrade::ArmorUpgrade()
    : Upgrade("Armor Upgrade", "Improves unit defense")
{
    this->set_level_scaling(1.15f); // 15% increase per level
    this->add_effect(UpgradeEffect{"armor", 1.5f, true});
    this->add_effect(UpgradeEffect{"health", 1.0f, true});
}

TrainingUpgrade::TrainingUpgrade()
    : Upgrade("Training Upgrade", "Improves unit production speed and experience gain")
{
    this->set_level_scaling(1.1f); // 10% increase per level
    this->add_effect(UpgradeEffect{"training_speed", 1.25f, true});
    this->add_effect(UpgradeEffect{"experience_gain", 1.2f, true});
}

ResourceUpgrade::ResourceUpgrade()
    : Upgrade("Resource Upgrade", "Improves resource gathering and storage")
{
    this->set_level_scaling(1.12f); // 12% increase per level
    this->add_effect(UpgradeEffect{"gathering_speed", 1.3f, true});
    this->add_effect(UpgradeEffect{"storage_capacity", 1.2f, true});
}

DefenseUpgrade::DefenseUpgrade()
    : Upgrade("Defense Upgrade", "Improves building health and repair speed")
{
    this->set_level_scaling(1.18f); // 18% increase per level
    this->add_effect(UpgradeEffect{"building_health", 2.0f, true});
    this->add_effect(UpgradeEffect{"repair_speed", 1.4f, true});
    this->add_effect(UpgradeEffect{"building_armor", 1.3f, true});
}