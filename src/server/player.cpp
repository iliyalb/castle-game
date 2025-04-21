#include "server/player.hpp"

Player::Player(PlayerID id, FactionType faction, Color color, float resource_handicap_percentage) : id_(id), faction_(faction), color_(color), resource_handicap_percentage_(resource_handicap_percentage)
{
}