
#include "CollisionManager.h"
#include "../game/Player.h"
#include "../game/Rabbit.h"
#include "../game/Bat.h"
#include "../game/Stage.h"
#include "../game/GameObject.h"

namespace
{
    constexpr int knockbackPowerX = 40;
    constexpr int knockbackPowerY = -5;
}

int CollisionManager::CheckCollisions(std::shared_ptr<Player>& m_pPlayer,
    std::vector<std::shared_ptr<Rabbit>>& m_pRabbits,
    std::vector<std::shared_ptr<Bat>>& m_pBats,
    std::shared_ptr<Stage>& m_pStage,
    int& killCount)
{
    int pointDelta = 0;
	killCount = 0;
    if (!m_pPlayer || !m_pStage) return pointDelta;

    Rect hitTileRect;

    // Rabbit×ステージ衝突
    for (auto& rabbit : m_pRabbits)
    {
        if (!rabbit) continue;

        Rect& rect = rabbit->GetRect();
        rabbit->SetOnGround(false);

        int iter = 0;
        const int kMaxIter = 3;
        while (iter < kMaxIter && m_pStage->IsCollision(rect, hitTileRect))
        {
            Vec2 push = rect.FixPos(hitTileRect);
            int tx = static_cast<int>(hitTileRect.GetX()) / m_pStage->GetChipW();
            int ty = static_cast<int>(hitTileRect.GetY()) / m_pStage->GetChipH();
            int tileId = m_pStage->GetData(tx, ty);

            // 壁に当たったら左右反転と指定されたチップに触れたら反転するように
            if (push.x != 0.0f)
            {
                rabbit->SetVelX(-rabbit->GetVelX());
            }
            if (tileId == 122 || tileId == 124)
            {
                rabbit->SetVelX(-rabbit->GetVelX());
            }

            if (push.y < 0.0f) //上から着地
            {
                rabbit->SetVelY(0.0f);
                rabbit->SetOnGround(true);
                rect.SetY(hitTileRect.GetTop() - rect.GetH() * 0.5f);
            }
            else if (push.y > 0.0f) //下から当たり
            {
                rabbit->SetVelY(0.0f);
                rabbit->SetOnGround(false);
            }

            ++iter;
        }

        //プレイヤー×Rabbit衝突
        if (m_pPlayer->IsHit(*rabbit))
        {
            Vec2 push = m_pPlayer->FixPos(*rabbit);

            if (!rabbit->IsDead)
            {
                if (m_pPlayer->IsHighJumpUnlock() || m_pPlayer->IsHighJumpActive())
                {
                    rabbit->OnDead();
                    pointDelta += 50;
					killCount += 1;
                }
                else
                {
                    if (!m_pPlayer->Isinvincible())
                    {
                        pointDelta -= 10;
                        m_pPlayer->StartInvincible(120);

                        Rect& playerRect = m_pPlayer->GetRect();
                        Rect& enemyRect = rabbit->GetRect();

                        // ノックバック
                        if (playerRect.GetX() < enemyRect.GetX())
                        {
                            playerRect.SetX(playerRect.GetX() - knockbackPowerX);
                            playerRect.SetY(playerRect.GetY() + knockbackPowerY);
                        }
                        else
                        {
                            playerRect.SetX(playerRect.GetX() + knockbackPowerX);
                            playerRect.SetY(playerRect.GetY() + knockbackPowerY);
                        }

                        m_pPlayer->SetVelY(knockbackPowerY);
                        m_pPlayer->SetOnGround(false);
                    }
                }
            }
        }
    }

    //プレイヤー×Bat衝突
    for (auto& bat : m_pBats)
    {
        if (!bat) continue;

        if (m_pPlayer->IsHit(*bat))
        {
            Vec2 push = m_pPlayer->FixPos(*bat);

            if (!bat->IsDead)
            {
                if (m_pPlayer->IsHighJumpUnlock()||m_pPlayer->IsHighJumpActive())
                {
                    bat->OnDead();
                    pointDelta += 50;
					killCount+= 1;
                }
                else
                {
                    if (!m_pPlayer->Isinvincible())
                    {
                        pointDelta -= 10;
                        m_pPlayer->StartInvincible(120);

                        Rect& playerRect = m_pPlayer->GetRect();
                        Rect& batRect = bat->GetRect();

                        if (playerRect.GetX() < batRect.GetX())
                        {
                            playerRect.SetX(playerRect.GetX() - knockbackPowerX);
                        }
                        else
                        {
                            playerRect.SetX(playerRect.GetX() + knockbackPowerX);
                        }

                        m_pPlayer->SetVelY(knockbackPowerY);
                        m_pPlayer->SetOnGround(false);
                    }
                }
            }
        }
    }

    //プレイヤー×ステージ衝突
    Rect& playerRect = m_pPlayer->GetRect();
    m_pPlayer->SetOnGround(false);

    int iter = 0; 
    const int kMaxIter = 3;

    while (iter < kMaxIter && m_pStage->IsCollision(playerRect, hitTileRect))
    {
        Vec2 push = playerRect.FixPos(hitTileRect);

        if (push.y < 0.0f) // 上から着地
        {
            float halfH = playerRect.GetH() * 0.5f;        // ← 0.5f
            playerRect.SetY(hitTileRect.GetTop() - halfH);
            m_pPlayer->SetVelY(0.0f);
            m_pPlayer->SetOnGround(true);
        }
        else if (push.y > 0.0f) // 下から当たり（破壊）
        {
            int tx = static_cast<int>(hitTileRect.GetX()) / m_pStage->GetChipW();
            int ty = static_cast<int>(hitTileRect.GetY()) / m_pStage->GetChipH();

			int tileId = m_pStage->GetData(tx, ty);
            if (tileId != 0 && tileId != 59 && tileId != 29 &&tileId != 81 && tileId != 51 && tileId != 73)
            {
				m_pStage->SetTile(tx, ty, 0);

                if (!m_pPlayer->IsHighJumpActive())
                    m_pPlayer->SetVelY(0.0f);

                m_pPlayer->TileBroke();
				//pointDelta += 10;
            }

            m_pPlayer->SetOnGround(false);
        }

        ++iter;
    }

    // Bat×ステージ衝突
    for (auto& bat : m_pBats)
    {
        if (!bat) continue;

        Rect& rect = bat->GetRect();
        bat->SetOnGround(false);

        iter = 0;
        while (iter < kMaxIter && m_pStage->IsCollision(rect, hitTileRect))
        {
            Vec2 push = rect.FixPos(hitTileRect);

            if (push.x != 0.0f)
                bat->SetVelX(-bat->GetVelX());

            if (push.y < 0.0f)
            {
                bat->SetVelY(0.0f);
                bat->SetOnGround(true);
                rect.SetY(hitTileRect.GetTop() - rect.GetH() * 0.5f);
            }
            else if (push.y > 0.0f)
            {
                bat->SetVelY(0.0f);
                bat->SetOnGround(false);
            }

            ++iter;
        }
    }

    return pointDelta;
}
