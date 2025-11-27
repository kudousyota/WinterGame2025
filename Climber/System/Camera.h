#pragma once
#include "Player.h"
#include <memory>

class Player;
class Camera
{
public:
	Camera();
	~Camera();
	void Init();
	void Update();
	void Draw();

	void InitCamera(std::shared_ptr<Camera> camera);
	void UpdateCamera(std::shared_ptr<Camera> camera, std::shared_ptr<Player> player);
	
	Vec2 GetCameraOffset()const;

private:

	Vec2 m_drawOffset;
	Vec2 m_pos;
	/*float drawOffsetX;
	float drawOffsetY;
	float posX;
	float posY;*/
	
};
//Player* m_pPlayer;


