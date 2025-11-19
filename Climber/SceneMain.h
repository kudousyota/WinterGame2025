#pragma once
#include <memory>
class Rect;
class Camera;
class Bg;
class Player;
class SceneMain
{
public:
	SceneMain();
	virtual ~SceneMain();

	virtual void Init();
	virtual void Update();
	virtual void Draw();

private:
	int m_frameCount;
	std::shared_ptr<Rect> m_pRect;
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<Bg> m_pBg;
};

