#include "PersonaEffect.h"
#include "Dxlib.h"
#include <cmath>
#include <algorithm> 

namespace 
{
    constexpr int kPersonaCutW = 1150;
	constexpr int kPersonaCutH = 1150;
}

PersonaEffect::PersonaEffect(const Position2& pos, int handle):
	Effect(pos),
	//渡されたハンドルを保存
	m_handle(handle),
	m_pos(pos),
	m_isDead(false),
	m_cameraOffset(0, 0),
	m_timer(0)
{
	m_pos = pos;
	//スライド位置初期化
	//画面外からスライドインするようにする
	m_SlideX = pos.x - 400.0f;
	m_SlideY = pos.y;
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

    switch (m_state)
    {
    case CutinState::Entrance:
    {
        // 入場（強めの追従 = 高速減速）
        // 調整ポイント：0.15f～0.30f
        m_SlideX += (m_pos.x - m_SlideX) * 0.22f;

        // 近づいたらとまる
        if (std::fabs(m_pos.x - m_SlideX) < 5.0f)
        {
            m_SlideX = m_pos.x;
            m_timer = 0;
            m_state = CutinState::Stay;
        }
        break;
    }
    case CutinState::Stay:
    {
        // 完全停止
        // 調整ポイント
        if (m_timer > 60)
        {
            m_timer = 0;
            m_state = CutinState::Exit;
        }
        break;
    }
    case CutinState::Exit:
    {
        // 退場斜め
        // 調整するときスピード8～16px/frame,Yは0～-8
        m_SlideX += 12.0f;
        m_SlideY -= 30.0f;

        // 調整するときフェード時間 12～24f
        if (m_timer > 20)
        {
            m_isDead = true;
        }
        break;
    }
    default:
        break;
    }
}

bool PersonaEffect::IsDead()const
{
	return m_isDead;
}
void PersonaEffect::Draw(const Vec2& cameraOffset)
{

    if (m_handle == -1) return;

    // スクリーン座標
    const int sx = static_cast<int>(m_SlideX + cameraOffset.x);
    const int sy = static_cast<int>(m_SlideY + cameraOffset.y);

    //  スケール/アルファをステート別に 
    // 基本は等倍
    float scale = 1.4f;
    // Entrance/Stay は不透明
    int alpha = 255;   

    if (m_state == CutinState::Exit)
    {
        // 退場中だけフェードアウト
        int a = 255 - m_timer * (255 / 20);
        if (a < 0) a = 0;
        alpha = a;

        // 少しだけ縮める伸ばす遊びできる
    }
    else if (m_state == CutinState::Entrance)
    {
        // 入場中、勢いを少し強調
        // 近づくにつれ等倍に戻る：距離に応じた縮小/拡大
        float dist = static_cast<float>(std::fabs(m_pos.x - m_SlideX));
        float t = max(1.0f, dist / 200.0f); // 0..1
    }

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
    DrawRectRotaGraph(sx, sy,
                       0,0,
                       kPersonaCutW,
                       kPersonaCutW,
		               scale,//拡大率
                       0.0f,
                       m_handle,
		               true
                      );
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

