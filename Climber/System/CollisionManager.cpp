#include "CollisionManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Stage.h"
#include "GameObject.h"

void CollisionManager::CheckCollisions(std::shared_ptr<Player> m_pPlayer, std::shared_ptr<Enemy> m_pEnemy, std::shared_ptr<Stage> m_pStage)
{
	// プレイヤーと敵の当たり判定をチェック
	if (m_pPlayer->IsHit(*m_pEnemy))
	{
		// 当たっていたら押し出し処理を行う
		Vec2 push = m_pPlayer->FixPos(*m_pEnemy);
		// プレイヤーの位置を更新
		m_pPlayer->GetRect().SetX(m_pPlayer->GetRect().GetX() + push.x);
		m_pPlayer->GetRect().SetY(m_pPlayer->GetRect().GetY() + push.y);
	}
	// プレイヤーとステージの当たり判定をチェック
	Rect hitTileRect;
	if (m_pStage->IsCollision(m_pPlayer->GetRect(), hitTileRect))
	{
		// マップチップと衝突した場合、プレイヤーを押し出す
		Vec2 push = m_pPlayer->GetRect().FixPos(hitTileRect);
		m_pPlayer->GetRect().SetX(m_pPlayer->GetRect().GetX() + push.x);
		m_pPlayer->GetRect().SetY(m_pPlayer->GetRect().GetY() + push.y);
	}
	// 敵とステージの当たり判定をチェック
	if (m_pStage->IsCollision(m_pEnemy->GetRect(), hitTileRect))
	{
		// マップチップと衝突した場合、敵を押し出す
		Vec2 push = m_pEnemy->GetRect().FixPos(hitTileRect);
		m_pEnemy->GetRect().SetX(m_pEnemy->GetRect().GetX() + push.x);
		m_pEnemy->GetRect().SetY(m_pEnemy->GetRect().GetY() + push.y);
	}
}