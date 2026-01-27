#include "CollisionManager.h"
#include "../game/Player.h"
#include "../game/Rabbit.h"
#include "../game/Bat.h"
#include "../game/Stage.h"
#include "../game/GameObject.h"
#include "SoundManager.h"
#include "DxLib.h"
#include <cassert>

namespace
{
    constexpr int knockbackPowerX = 40;
    constexpr int knockbackPowerY = -5;
}


void CollisionManager::Init()
{
    m_effectHandle = LoadGraph("data/Effect.png");
    

    // 画像サイズから列・行を算出（16x16固定）
    int w = 0, h = 0;
    if (m_effectHandle >= 0 && GetGraphSize(m_effectHandle, &w, &h) == 0) {
        m_sheetCols = (w / kCutW);
        m_sheetRows = (h / kCutH);
    }
    else {
        m_sheetCols = m_sheetRows = 0;
    }

    m_tileBreakEffects.clear();
}


void CollisionManager::Update()
{
    // 各インスタンスの経過フレーム更新と終了判定
    for (auto& e : m_tileBreakEffects) {
        if (e.finished) continue;
        e.localFrame++;

        int currentIndex = e.localFrame / e.frameStep; // 0,1,2...
        if (currentIndex >= e.frames) {
            e.finished = true;
        }
    }

    // 終わったものを削除
    m_tileBreakEffects.erase(
        std::remove_if(m_tileBreakEffects.begin(), m_tileBreakEffects.end(),
                       [](const EffectInstance& e){ return e.finished; }),
        m_tileBreakEffects.end()
    );
}


void CollisionManager::Draw()
{
    if (m_effectHandle < 0) return;

    for (const auto& e : m_tileBreakEffects) {
        int idx = e.localFrame / e.frameStep; // 0..frames-1
        if (idx < 0) idx = 0;
        if (idx >= e.frames) idx = e.frames - 1;

        // 現在コマの「列」を開始列から加算（横方向にアニメする前提）
        int col = e.startCol + idx;
        int row = e.startRow;

        // はみ出し防止
        if (col >= m_sheetCols) {
            // 次の列に行けない場合は安全にクランプ（or finished=true にしてもOK）
            col = m_sheetCols - 1;
        }
        if (row >= m_sheetRows) continue;

        int cutX = col * kCutW;
        int cutY = row * kCutH;

        // タイル中央にきれいに置きたい場合はオフセット調整
        int sx = static_cast<int>(e.x - m_cameraX - kCutW * 0.5f);
        int sy = static_cast<int>(e.y - m_cameraY - kCutH * 0.5f);
        // 透過ブレンド（他でブレンドが変わっていなければ不要）
        // SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
        DrawRectGraph(sx, sy, cutX, cutY, kCutW, kCutH, m_effectHandle, true);
        // SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
}

void CollisionManager::AddTileBreakEffect(float px, float py,
    int startCol, int startRow,
    int frames,
    int frameStep)
{

    // シート範囲に収める
    if (m_sheetCols <= 0 || m_sheetRows <= 0) return;
    if (startCol < 0 || startCol >= m_sheetCols) return;
    if (startRow < 0 || startRow >= m_sheetRows) return;
    if (frames <= 0) return;

    EffectInstance e;
    e.x = px;
    e.y = py;
    e.startCol = startCol;
    e.startRow = startRow;
    e.frames = frames;
    e.frameStep = std::max<int>(1, frameStep);
    e.localFrame = 0;
    e.finished = false;

    m_tileBreakEffects.push_back(e);

}

int CollisionManager::CheckCollisions(std::shared_ptr<Player>& m_pPlayer,
    std::vector<std::shared_ptr<Rabbit>>& m_pRabbits,
    std::vector<std::shared_ptr<Bat>>& m_pBats,
    std::shared_ptr<Stage>& m_pStage,
    int& killCount,
    CollisionManager* effectSink)
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

					SoundManager::PlaySE("Kill");
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

					SoundManager::PlaySE("Kill");
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
            float halfH = playerRect.GetH() * 0.5f;
            playerRect.SetY(hitTileRect.GetTop() - halfH);
            m_pPlayer->SetVelY(0.0f);
            m_pPlayer->SetOnGround(true);
        }
        else if (push.y > 0.0f) // 下から当たり
        {
            int tx = static_cast<int>(hitTileRect.GetX()) / m_pStage->GetChipW();
            int ty = static_cast<int>(hitTileRect.GetY()) / m_pStage->GetChipH();
            int tileId = m_pStage->GetData(tx, ty);

        
           
            if (tileId != 0 && tileId != 59 && tileId != 29 && tileId != 81 && tileId != 51 && tileId != 73)
            {
                m_pStage->SetTile(tx, ty, 0);
                SoundManager::PlaySE("TileBreak");
                // ここで演出を積む
                if (effectSink) {
                    float px = tx * m_pStage->GetChipW() + m_pStage->GetChipW() * 0.5f;
                    float py = ty * m_pStage->GetChipH() + m_pStage->GetChipH() * 0.5f;

                    effectSink->AddTileBreakEffect(
                        px, py,
                        /*startCol*/ 8,
                        /*startRow*/ 1,
                        /*frames*/   4,
                        /*frameStep*/4
                    );
                }

                if (!m_pPlayer->IsHighJumpActive())
                    m_pPlayer->SetVelY(0.0f);

                m_pPlayer->TileBroke();
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
