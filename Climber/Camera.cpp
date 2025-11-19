#include "Dxlib.h"
#include "Camera.h"
#include "Game.h"
#include <cmath>

VECTOR VLerp(VECTOR& start, VECTOR& end, float t)
{
	VECTOR ret;
	ret.x = std::lerp(start.x, end.x, t);
	ret.y = std::lerp(start.y, end.y, t);
	ret.z = std::lerp(start.z, end.z, t);

	return ret;
}

void InitCamera(Camera& camera)
{
	camera.drawOffset = VGet(0,0,0);
	camera.pos = VGet(0, 0, 0);
	/*camera.posX;
	camera.posY;*/
}

void UpdateCamera(Camera& camera, const std::shared_ptr<Player> player)
{
	//camera.pos = m_pPlayer->GetRect().GetX();
	//プレイヤーとカメラの位置を同じにする
	/*camera.posX = player.GetRect().GetX();
	camera.posY = player.GetRect().GetY();*/
	VECTOR playerPos = VGet(player->GetRect().GetX(), player->GetRect().GetY(), player->GetRect().GetZ());
	//カメラとプレイヤーの位置を同じにする
	camera.pos = VLerp(camera.pos, playerPos, 0.9f);

	//反転する
	camera.drawOffset.x = camera.pos.x * -1;
	camera.drawOffset.y = camera.pos.y * -1;
	// その時、画面の中央にプレイヤーが来るようにする
	// (camera.posが画面の中央になるようにする)
	camera.drawOffset.x += Game::kScreenWidth/ 2;
	camera.drawOffset.y += Game::kScreenHeight / 2;
}
