#pragma once
#include "Player.h"
#include <memory>

class Player;
struct Camera
{
	VECTOR drawOffset;
	VECTOR pos;
	/*float drawOffsetX;
	float drawOffsetY;
	float posX;
	float posY;*/
};

void InitCamera(Camera& camera);
void UpdateCamera(Camera& camera, const std::shared_ptr<Player> player);

//Player* m_pPlayer;


