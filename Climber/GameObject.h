#pragma once
#include <vector>
#include "Rect.h"

class Player;
class GameObject {
public:
    GameObject();
    virtual ~GameObject();

    void Init();
    void Update(Player& player);
    void Draw();

private:
    int m_blocks;
    Rect m_rect;   // 当たり判定用
    bool m_isHit;  // プレイヤーと衝突しているか
};


