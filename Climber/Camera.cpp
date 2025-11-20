#include "Dxlib.h"
#include "Camera.h"
#include "Game.h"
#include <cmath>

Vec2 VLerp(Vec2& start, Vec2& end, float t)
{
	Vec2 ret;
	ret.x = std::lerp(start.x, end.x, t);
	ret.y = std::lerp(start.y, end.y, t);

	return ret;
}



Camera::Camera():
	m_drawOffset{0,0},
	m_pos{0,0}
{
}

Camera::~Camera()
{
}

void Camera::Init()
{

}
void Camera::Update()
{

}
void Camera::Draw()
{

}
void Camera::InitCamera(std::shared_ptr<Camera>camera)
{
	m_drawOffset = {0,0};
	m_pos = {0,0};
	/*camera.posX;
	camera.posY;*/
}

void Camera::UpdateCamera(std::shared_ptr<Camera> camera,std::shared_ptr<Player> player)
{
	//camera.pos = m_pPlayer->GetRect().GetX();
	//プレイヤーとカメラの位置を同じにする
	/*camera.posX = player.GetRect().GetX();
	camera.posY = player.GetRect().GetY();*/

	//プレイヤーのポジションを取得してカメラのポジションに使用する
	auto playerX=player->GetPos().x;
	auto playerY=player->GetPos().y;
	Vec2 playerPos = { playerX,playerY };

	//カメラとプレイヤーの位置を同じにする
	m_pos = VLerp(m_pos, playerPos, 0.9f);

	//反転する
	m_drawOffset.x = m_pos.x * -1;
	m_drawOffset.y = m_pos.y * -1;
	// その時、画面の中央にプレイヤーが来るようにする
	// (camera.posが画面の中央になるようにする)
	m_drawOffset.x += Game::kScreenWidth / 2;
	m_drawOffset.y += Game::kScreenHeight / 2;
}

Vec2 Camera::GetCameraOffset()
{
	return {m_drawOffset.x,m_drawOffset.y};
}
