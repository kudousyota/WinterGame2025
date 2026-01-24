#include"Particle.h"
#include<cmath>
#include <vector>

std::vector<Particle> g_particles;
int Particle::s_baseTexSize = 8;

Particle::Particle(float x,float y,float vx,float vy,float lifeSec,float sizePx,unsigned int colorARGB,int texHandle,float rotRad,float rotVelRad):
m_x(x),
m_y(y),
m_vx(vx),
m_vy(vy),
m_life(lifeSec),
m_maxLife(lifeSec),
m_size(sizePx),
m_colorARGB(colorARGB),
m_tex(texHandle),
m_rot(rotRad),m_rotVel(rotVelRad),
m_useSubRect(false)

{
    m_alpha = 255;
}

Particle Particle::MakeTilePiece(float centerX, float centerY, int tilesetHandle, int sx, int sy, int sw, int sh, float vx, float vy, float lifeSec, unsigned int colorARGB)
{

    Particle p(centerX, centerY, vx, vy, lifeSec, 1.f, colorARGB);
    p.m_useSubRect = true;
    p.m_tileset = tilesetHandle;
    p.m_sx = sx; p.m_sy = sy; p.m_sw = sw; p.m_sh = sh;
    return p;

}


void Particle::Update(float dt, float gravity)
{
    m_vy += gravity * dt;
    m_x += m_vx * dt;
    m_y += m_vy * dt;
    m_rot += m_rotVel * dt;
    m_life -= dt;

    // αフェード
    float t = 1.f - (m_life / m_maxLife);
    if (t < 0) t = 0; else if (t > 1) t = 1;
    int a = static_cast<int>(255.f * (1.f - t * t));
    if (a < 0) a = 0; else if (a > 255) a = 255;
    m_alpha = a;
}

void Particle::Draw(const Vec2& cameraffset) const
{

    if (m_alpha <= 0) return;

    const float sx = m_x + cameraffset.x;
    const float sy = m_y + cameraffset.y;


    SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);

    if (m_useSubRect && m_tileset >= 0)
    {
        // タイルサブ矩形

        float dx = sx - m_sw * 0.5f;  
        float dy = sy - m_sh * 0.5f; 

        DrawRectGraphF(dx, dy, m_sx, m_sy, m_sw, m_sh, m_tileset, TRUE);
    }
    else if (m_tex >= 0) 
    {
        // 小テクスチャ（白丸など）を回転・拡大して描画
        double scale = static_cast<double>(m_size) / static_cast<double>(s_baseTexSize);
        DrawRotaGraphF(sx, sy, scale, m_rot, m_tex, TRUE);
    }
    else 
    {
        // テクスチャなし → 白四角
        int r = (m_colorARGB >> 16) & 0xFF;
        int g = (m_colorARGB >> 8) & 0xFF;
        int b = (m_colorARGB) & 0xFF;
        unsigned col = GetColor(r, g, b);
        float half = m_size * 0.5f;
        DrawBoxAA(sx - half, sy - half, sx + half, sy + half, col, TRUE);
    }

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

bool Particle::IsDead() const 
{
    return m_life <= 0.f;
}

void Particle::SetBaseTexSize(int px)
{
    s_baseTexSize = (px > 0 ? px : 8);
}

