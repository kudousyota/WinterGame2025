#include "CollisionManager.h"
#include "Player.h"
#include "Rabbit.h"
#include "Bat.h"
#include "Stage.h"
#include "GameObject.h"

void CollisionManager::CheckCollisions(std::shared_ptr<Player> m_pPlayer, std::shared_ptr<Rabbit> m_pRabbit, std::shared_ptr<Bat>m_pBat, std::shared_ptr<Stage> m_pStage)
{
	// プレイヤーとうさぎの当たり判定をチェック
	if (m_pPlayer->IsHit(*m_pRabbit))
	{
		// FixPosは自身のRectを修正して押し出し量を返すここで位置を二重に加算しない
		Vec2 push = m_pPlayer->FixPos(*m_pRabbit);
		// 必要なら push を見て速度を調整する
	}
	// プレイヤーとコウモリの当たり判定をチェック
	if (m_pPlayer->IsHit(*m_pBat))
	{
		// FixPosは自身のRectを修正して押し出し量を返すここで位置を二重に加算しない
		Vec2 push = m_pPlayer->FixPos(*m_pBat);
	}

	// プレイヤーとステージの当たり判定をチェック
	Rect hitTileRect;

	// プレイヤー反復処理
	Rect& playerRect = m_pPlayer->GetRect();
	m_pPlayer->SetOnGround(false); // 毎フレーム最初に false にして、下方向の押し戻しがあれば true にする

	int iter = 0;
	const int kMaxIter = 3; // 必要なら増やす

	while (iter < kMaxIter && m_pStage->IsCollision(playerRect, hitTileRect))
	{
		Vec2 push = playerRect.FixPos(hitTileRect); // AABB 純押し出し

		// 床（上から押し戻された）
		if (push.y < 0.0f)
		{
			// タイルの上にスナップして微小な重なりを残さない
			const float playerHalfH = playerRect.GetH() * 0.5f;
			const float tileTop = hitTileRect.GetTop(); // ワールド座標のタイル上端
			const float snapY = tileTop - playerHalfH;
			playerRect.SetY(snapY);

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
				if (oldId != 0) 
				{
					m_pStage->SetTile(tx, ty, 0);
					// 下から当たったので縦速度をクリア
					m_pPlayer->SetVelY(0.0f);
					//ハイジャンプの場合
					if (!m_pPlayer->IsHighJumpNum())
					{
						m_pPlayer->SetVelY(0.0f);
					}
					m_pPlayer->TileBroke();   //ここが解禁カウントのトリガ
				}
			}
			// 下から当たった場合は着地にはしない（上方向の衝突）
			m_pPlayer->SetOnGround(false);
		}

		++iter;
	}
	// ここで iter==0 なら当たりなしm_onGround は false のまま

	// 敵とステージの当たり判定
	Rect& enemyRect = m_pRabbit->GetRect();
	Rect& enemyBatRect = m_pBat->GetRect();

	m_pRabbit->SetOnGround(false); // 毎フレーム最初に false に
	m_pBat->SetOnGround(false);
	iter = 0;
	const int kMaxIterEnemy = 3; // 速度が大きいなら 4〜6 でも

	while (iter < kMaxIterEnemy && m_pStage->IsCollision(enemyRect, hitTileRect))
	{
		Vec2 push = enemyRect.FixPos(hitTileRect); // AABB純押し出し

		// 横壁
		if (push.x != 0.0f)
		{
			// 巡回反転
			float inversion = m_pRabbit->GetVelX();
			m_pRabbit->SetVelX(-inversion);
		}

		// 床天井Y速度を停止、接地フラグ、スナップ
		if (push.y < 0.0f)
		{
			// 床に乗った（下方向に押し戻された）
			m_pRabbit->SetVelY(0.0f);
			m_pRabbit->SetOnGround(true);

			// 中心原点のスナップ（Rect中心座標前提）
			const float halfH = enemyRect.GetH() * 0.5f;
			enemyRect.SetY(hitTileRect.GetTop() - halfH);
		}
		else if (push.y > 0.0f) {
			// 天井
			m_pRabbit->SetVelY(0.0f);
			m_pRabbit->SetOnGround(false);
		}

		++iter;
	}
	iter = 0;
	// コウモリ反復処理
	while (iter < kMaxIterEnemy && m_pStage->IsCollision(enemyBatRect, hitTileRect))
	{
		Vec2 push = enemyBatRect.FixPos(hitTileRect); // AABB純押し出し

		// 横壁
		if (push.x != 0.0f)
		{
			// 巡回反転
			float inversion = m_pBat->GetVelX();
			m_pBat->SetVelX(-inversion);
		}

		// 床天井Y速度を停止、接地フラグ、スナップ
		if (push.y < 0.0f)
		{
			// 床に乗った（下方向に押し戻された）
			m_pBat->SetVelY(0.0f);
			m_pBat->SetOnGround(true);

			// 中心原点のスナップ（Rect中心座標前提）
			const float halfH = enemyBatRect.GetH() * 0.5f;
			enemyBatRect.SetY(hitTileRect.GetTop() - halfH);
		}
		else if (push.y > 0.0f) {
			// 天井
			m_pBat->SetVelY(0.0f);
			m_pBat->SetOnGround(false);
		}

		++iter;
	}
}