#include "AnimationManager.h"
#include "DxLib.h"
#include <algorithm>
#include "PersonaEffect.h"


AnimationManager::AnimationManager()
	
{
	m_handles.push_back(-1);
	m_handles.push_back(LoadGraph("data/PersonaCut.png"));
}

AnimationManager::~AnimationManager()
{
}

void AnimationManager::Create(const Position2& pos, EffectType type)
{
	switch (type)
	{
	case EffectType::persona:
		m_effect.push_back(std::make_shared<PersonaEffect>(pos, m_handles[1]));
		break;
	default:
		break;
	}
}

void AnimationManager::Init()
{

}
void AnimationManager::Update()
{
	for (auto& effect: m_effect)
	{
		effect->Update();
	}
	m_effect.remove_if([](auto effect)
	{
	    return effect->IsDead();
	});

}
void AnimationManager::Draw()
{
	for (auto& effect: m_effect )
	{
		effect->Draw();
	}
}
