#pragma once
#include "Vec2.h"
#include "DxLib.h"

class Particle 
{
public:
    // 簡易破片／ダスト用の生成コンストラクタ
    Particle(float x, float y,
        float vx, float vy,
        float lifeSec,
        float sizePx,
        unsigned int colorARGB,
        int texHandle = -1,
        float rotRad = 0.f,
        float rotVelRad = 0.f);

    // タイルのサブ矩形破片を生成するファクトリ
    static Particle MakeTilePiece(float centerX, float centerY,
        int tilesetHandle,
        int sx, int sy, int sw, int sh,
        float vx, float vy,
        float lifeSec,
        unsigned int colorARGB = 0xFFFFFFFF);

    void Update(float dt, float gravity = 600.f);
    void Draw(const Vec2& cameraffset) const;
    bool IsDead() const;

    static void SetBaseTexSize(int px);



private:
    // 位置・速度・回転
    float m_x{}, m_y{};
    float m_vx{}, m_vy{};
    float m_rot{}, m_rotVel{};

    // 寿命
    float m_life{}, m_maxLife{};
    int   m_alpha{ 255 };

    // 見た目
    float m_size{ 8.f };
    unsigned int m_colorARGB{ 0xFFFFFFFF };
    int   m_tex{ -1 };

    // タイルサブ矩形モード
    bool  m_useSubRect{ false };
    int   m_tileset{ -1 };
    int   m_sx{ 0 }, m_sy{ 0 }, m_sw{ 0 }, m_sh{ 0 };

    static int s_baseTexSize;
};

