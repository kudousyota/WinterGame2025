#include "AnimationManager.h"
#include "DxLib.h"

AnimationManager::AnimationManager():
	m_personaHandle(-1)
{
	m_personaHandle = LoadGraph("data/PersonaCut.png");
}

AnimationManager::~AnimationManager()
{
}

void AnimationManager::Init()
{

}
void AnimationManager::Update()
{
	
}
void AnimationManager::Draw()
{
}
