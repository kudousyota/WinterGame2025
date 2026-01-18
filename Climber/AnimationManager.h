#pragma once
#include <memory>

class Player;
class AnimationManager
{
	public:
	AnimationManager();
	~AnimationManager();
	void Init();
	void Update();
	void Draw();

public:
	//
	std::shared_ptr<Player> m_pPlayer;


	// アニメーション用変数
	int m_personaHandle;
};

