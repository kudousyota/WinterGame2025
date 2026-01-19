#include "PersonaEffect.h"
#include "Dxlib.h"


PersonaEffect::PersonaEffect(const Position2& pos, int handle):
	Effect(pos),
	m_handle(-1),
	m_pos(0,0),
	m_timer(0)
{
}
PersonaEffect::~PersonaEffect()
{

}
void PersonaEffect::Init()
{

}
void PersonaEffect::Update()
{
	m_timer++;
	if (m_timer > 40)
	{
		//è¡Ç¶ÇÈÇÊÇ§Ç…Ç∑ÇÈ
		m_isDead = true;
	}
}
void PersonaEffect::Draw()
{
	//ÇøÇ¿Çﬁ
	float scale = 1.0f + (40 - m_timer) * 0.03f;
	//èôÅXÇ…ìßñæÇ…Ç»ÇÈ
	int alpha = 255 - m_timer * 6;
	if (alpha < 0)
	{
		alpha = 0;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	DrawRotaGraph(
		static_cast<int>(m_pos.x),
		static_cast<int>(m_pos.y),
		scale,
		0.0f,
		m_handle,
		TRUE
	);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


}
