#pragma once

struct UnitStats
{
    int max_health;
    int attack;
    int armor;
    int speed;
    int cost_gold;
    int cost_lumber;

    UnitStats(int health, int atk, int def, int spd, int gold, int lumber)
        : max_health(health), attack(atk), armor(def), speed(spd),
          cost_gold(gold), cost_lumber(lumber) {}
};