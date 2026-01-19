#pragma once
#include "Effect.h"
#include "../System/Camera.h"
#include "../System/Vec2.h"

class Camera;
class PersonaEffect:public Effect
{
public:
	PersonaEffect(const Position2& pos, int handle);
	~PersonaEffect();
	void Init();
	void Update() override;
	void Draw(const Vec2& cameraOffset) override;
	bool IsDead()const;
protected:
	bool m_isDead = false;
private:
	enum class CutinState
	{
		//“üê
		Entrance,
		//‘Øİ
		Stay,
		//‘Şê
		Exit
	};

	CutinState m_state = CutinState::Entrance;
	//ƒXƒ‰ƒCƒh
	float m_SlideX;
	float m_SlideY;

	Position2 m_pos;
	Position2 m_cameraOffset;
	int m_handle;
	int m_timer;

};

