#pragma once
#include"Geometry.h"
#include <memory>
#include <list>
#include "Effect.h"
#include <vector>

class Player;
//エフェクト
class Effect;
class Camera;
enum  class EffectType
{
	//何もしない
	none,
	damage,
	destruction,
	persona,

};

class AnimationManager
{
public:
	AnimationManager();
	virtual~AnimationManager();
	virtual void Create(const Position2& pos, EffectType type);
	virtual void Init();
	virtual void Update();
	virtual void Draw(const Vec2& cameraOffset);

private:
	//エフェクトを持つ
	std::vector<int> m_handles;
	std::list<std::shared_ptr<Effect>> m_effect;
	std::shared_ptr<Player> m_pPlayer;
};

