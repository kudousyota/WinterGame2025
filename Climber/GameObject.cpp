#include "GameObject.h"
#include "Player.h"
#include <DxLib.h>
#include <cmath>

GameObject::GameObject():
m_blocks(0),
m_isHit(false)
{

}

GameObject::~GameObject()
{
}
void GameObject::Init()
{
	//“–‚½‚è”»’è—p‚Ì‹éŒ`‚ğ‰Šú‰»
	m_rect.Init(400.0f, 520.0f, 100.0f, 100.0f);
}
void GameObject::Update(Player& player)
{
	m_isHit = m_rect.IsHit(player.GetRect());
}
void GameObject::Draw()
{
    float x = m_rect.GetX();
    float y = m_rect.GetY();
    float w = m_rect.GetW();
    float h = m_rect.GetH();

    int left = static_cast<int>(x - w * 0.5f);
    int top = static_cast<int>(y - h * 0.5f);
    int right = static_cast<int>(x + w * 0.5f);
    int bottom = static_cast<int>(y + h * 0.5f);

    DrawBox(left, top, right, bottom, GetColor(255,0,0), false);
}