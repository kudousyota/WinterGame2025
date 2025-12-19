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
		// FixPosは自身のRectを修正して押し出し量を返すここで位置を二重に加算しない
		Vec2 push = m_pPlayer->FixPos(*m_pEnemy);
		// 必要なら push を見て速度を調整する（今回は位置だけで十分）
	}

	// プレイヤーとステージの当たり判定をチェック
	Rect hitTileRect;
	if (m_pStage->IsCollision(m_pPlayer->GetRect(), hitTileRect))
	{
		// FixPos はプレイヤーの Rect を修正済み、返り値は実際に移動した量
		Vec2 push = m_pPlayer->GetRect().FixPos(hitTileRect);

		// 着地と下から破壊判定
		if (push.y < 0.0f)
		{
			// タイルの上にスナップして微小な重なりを残さない
			const float playerHalfH = m_pPlayer->GetRect().GetH() * 0.5f;
			const float tileTop = hitTileRect.GetTop(); // ワールド座標のタイル上端
			const float snapY = tileTop - playerHalfH;
			m_pPlayer->GetRect().SetY(snapY);

			// 縦速度クリアして着地フラグを立てる
			m_pPlayer->SetVelY(0.0f);
			m_pPlayer->SetOnGround(true);
		}
		else if (push.y > 0.0f)
		{
			// 下から当たった -> 当該タイルを破壊する
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

	// 敵とステージの当たり判定（敵についても同様に FixPos を使う）
	if (m_pStage->IsCollision(m_pEnemy->GetRect(), hitTileRect))
	{
		// FixPos を使って敵の Rect を直接修正する（追加の Set は不要）
		Vec2 push = m_pEnemy->GetRect().FixPos(hitTileRect);
		// 必要に応じて敵の速度や行動を調整する
	}
}