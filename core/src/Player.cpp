#include "Player.h"


int Player::maxId = 0;

Player::Player()
{
	this->id = maxId++;
}

Player::~Player()
{
}