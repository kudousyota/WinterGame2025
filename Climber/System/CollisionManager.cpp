#include "CollisionManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Stage.h"
#include "GameObject.h"

void CollisionManager::CheckCollisions(std::shared_ptr<Player> m_pPlayer, std::shared_ptr<Enemy> m_pEnemy, std::shared_ptr<Stage> m_pStage)
{
	// プレイヤーと敵の当たり判定
	if (m_pPlayer->IsHit(*m_pEnemy))
	{
		Vec2 push = m_pPlayer->FixPos(*m_pEnemy);
		m_pPlayer->GetRect().SetX(m_pPlayer->GetRect().GetX() + push.x);
		m_pPlayer->GetRect().SetY(m_pPlayer->GetRect().GetY() + push.y);
	}

	// プレイヤーとステージの当たり判定
	Rect hitTileRect;
	if (m_pStage->IsCollision(m_pPlayer->GetRect(), hitTileRect))
	{
		Vec2 push = m_pPlayer->GetRect().FixPos(hitTileRect);
		m_pPlayer->GetRect().SetX(m_pPlayer->GetRect().GetX() + push.x);
		m_pPlayer->GetRect().SetY(m_pPlayer->GetRect().GetY() + push.y);

		// 着地と下から破壊判定
		if (push.y < 0.0f)
		{
			m_pPlayer->SetVelY(0.0f);
			m_pPlayer->SetOnGround(true);
		}
		else if (push.y > 0.0f)
		{
			const int tileW = m_pStage->GetChipW();
			const int tileH = m_pStage->GetChipH();
			if (tileW > 0 && tileH > 0)
			{
				int tx = static_cast<int>(hitTileRect.GetX() / tileW);
				int ty = static_cast<int>(hitTileRect.GetY() / tileH);
				m_pStage->SetTile(tx, ty, 0);
			}
			// 下から当たった場合は着地にはしない
			m_pPlayer->SetOnGround(false);
		}
	}
	else
	{
		// タイルに触れていなければ地面フラグを解除
		m_pPlayer->SetOnGround(false);
	}

	// 敵とステージの当たり判定
	if (m_pStage->IsCollision(m_pEnemy->GetRect(), hitTileRect))
	{
		Vec2 push = m_pEnemy->GetRect().FixPos(hitTileRect);
		m_pEnemy->GetRect().SetX(m_pEnemy->GetRect().GetX() + push.x);
		m_pEnemy->GetRect().SetY(m_pEnemy->GetRect().GetY() + push.y);
	}
}