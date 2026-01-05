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
		// 必要なら push を見て速度を調整する
	}

	// プレイヤーとステージの当たり判定をチェック
	Rect hitTileRect;
	if (m_pStage->IsCollision(m_pPlayer->GetRect(), hitTileRect))
	{
		// FixPos はプレイヤーのrectを修正返り値は実際に移動した量
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

				uint8_t oldId = m_pStage->GetData(tx, ty);
				if (oldId != 0) {
					m_pStage->SetTile(tx, ty, 0);
					m_pPlayer->TileBroke();   //ここが解禁カウントのトリガ
				}

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
	
		Rect& enemyRect = m_pEnemy->GetRect(); 

		m_pEnemy->SetOnGround(false);         // 毎フレーム最初に false に

		int iter = 0;
		const int kMaxIter = 3;               // 速度が大きいなら 4〜6 でも

		while (iter < kMaxIter && m_pStage->IsCollision(enemyRect, hitTileRect))
		{
			Vec2 push = enemyRect.FixPos(hitTileRect);  // AABB純押し出し

			// 横壁
			if (push.x != 0.0f) 
			{
				// 巡回反転
				float inversion = m_pEnemy->GetVelX();
				m_pEnemy->SetVelX(-inversion);
			}

			// 床天井Y速度を停止、接地フラグ、スナップ
			if (push.y < 0.0f)
			{
				// 床に乗った（下方向に押し戻された）
				m_pEnemy->SetVelY(0.0f);
				m_pEnemy->SetOnGround(true);

				// 中心原点のスナップ（Rect中心座標前提）
				const float halfH = enemyRect.GetH() * 0.5f;
				enemyRect.SetY(hitTileRect.GetTop() - halfH);
			}
			else if (push.y > 0.0f) {
				// 天井
				m_pEnemy->SetVelY(0.0f);
				m_pEnemy->SetOnGround(false);
			}

			++iter;
		}

}

	
