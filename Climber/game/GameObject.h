#pragma once
#include <vector>
#include "../System/Rect.h"

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
    // 当たり判定用
    Rect m_rect;  
    // プレイヤーと衝突しているか
    bool m_isHit; 
};


