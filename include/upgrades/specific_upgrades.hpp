#pragma once

#include "upgrade.hpp"

class WeaponUpgrade : public Upgrade
{
public:
    WeaponUpgrade();
};

class ArmorUpgrade : public Upgrade
{
public:
    ArmorUpgrade();
};

class TrainingUpgrade : public Upgrade
{
public:
    TrainingUpgrade();
};

class ResourceUpgrade : public Upgrade
{
public:
    ResourceUpgrade();
};

class DefenseUpgrade : public Upgrade
{
public:
    DefenseUpgrade();
};