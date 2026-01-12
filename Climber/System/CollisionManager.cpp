#include "CollisionManager.h"
#include "../game/Player.h"
#include "../game/Rabbit.h"
#include "../game/Bat.h"
#include "../game/Stage.h"
#include "../game/GameObject.h"

namespace
{
	constexpr int knockbackPpwerX = 40;
	constexpr int knockbackPpwerY = -5;
}
int CollisionManager::CheckCollisions(std::shared_ptr<Player> m_pPlayer, std::shared_ptr<Rabbit> m_pRabbit, std::shared_ptr<Bat>m_pBat, std::shared_ptr<Stage> m_pStage)
{
	int pointDelta = 0;

	if(!m_pPlayer || !m_pStage)
	{
		return pointDelta; // 必須オブジェクトがない場合は早期終了
	}

	// プレイヤーとうさぎの当たり判定
	if (m_pRabbit && m_pPlayer->IsHit(*m_pRabbit))
	{
		// 押し出し（AABB 補正）
		Vec2 push = m_pPlayer->FixPos(*m_pRabbit);

		// 敵がまだ生きているなら処理
		if (!m_pRabbit->IsDead)
		{
			// ハイジャンプが解放されているときのみ敵を倒す
			if (m_pPlayer->IsHighJumpUnlock())
			{
				m_pRabbit->OnDead();
				// 倒したときの加点
				pointDelta += 50;
			}
			else
			{
				// ハイジャンプ未解放被弾扱い無敵でなければ減点して無敵を開始
				if (!m_pPlayer->Isinvincible())
				{
					pointDelta -= 10;
					// 無敵開始
					m_pPlayer->StartInvincible(120); // 60フレーム無敵

					// ノックバック処理
					// 方向はプレイヤーと敵の相対位置で決定（プレイヤーが左なら左へ押し戻す）
					Rect& playerRect = m_pPlayer->GetRect();
					Rect& enemyRect = m_pRabbit->GetRect();

					if (playerRect.GetX() < enemyRect.GetX())
					{
						// プレイヤーが敵の左側 -> 左へ押す
						playerRect.SetX(playerRect.GetX() - knockbackPpwerX);
						playerRect.SetY(playerRect.GetY() + knockbackPpwerY);
					}
					else
					{
						// 右側
						playerRect.SetX(playerRect.GetX() + knockbackPpwerX);
						playerRect.SetY(playerRect.GetY() + knockbackPpwerY);
					}

					// 上向きの速度を与える（ジャンプ方向）
					m_pPlayer->SetVelY(knockbackPpwerY);

					// 接地フラグ解除
					m_pPlayer->SetOnGround(false);
				}
			}
		}
	}

	// プレイヤーとコウモリの当たり判定をチェック
	if (m_pBat && m_pPlayer->IsHit(*m_pBat))
	{
		Vec2 push = m_pPlayer->FixPos(*m_pBat);

		if (!m_pBat->IsDead)
		{
			if (m_pPlayer->IsHighJumpUnlock())
			{
				m_pBat->OnDead();
				pointDelta += 50;
			}
			else
			{
				if (!m_pPlayer->Isinvincible())
				{
					pointDelta -= 10;
					m_pPlayer->StartInvincible(60);

					// ノックバック（Bat）
					const float knockbackX = knockbackPpwerX;
					const float knockbackY = knockbackPpwerY;
					Rect& playerRect = m_pPlayer->GetRect();
					Rect& batRect = m_pBat->GetRect();

					if (playerRect.GetX() < batRect.GetX())
					{
						playerRect.SetX(playerRect.GetX() - knockbackX);
					}
					else
					{
						playerRect.SetX(playerRect.GetX() + knockbackX);
					}

					m_pPlayer->SetVelY(knockbackY);
					m_pPlayer->SetOnGround(false);
				}
			}
		}
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
				// 当たったタイルのタイル座標を計算
				int tx = static_cast<int>(hitTileRect.GetX() / tileW);
				int ty = static_cast<int>(hitTileRect.GetY() / tileH);

				uint8_t oldId = m_pStage->GetData(tx, ty);
				if (oldId != 0)
				{
					m_pStage->SetTile(tx, ty, 0);
					// 下から当たった際、ハイジャンプ中は縦速度を消さない
					if (!m_pPlayer->IsHighJumpActive())
					{
						m_pPlayer->SetVelY(0.0f);
					}
					//ここが解禁カウントのトリガ
					m_pPlayer->TileBroke();
				}
			}
			// 下から当たった場合は着地にはしない(上方向の衝突）
			m_pPlayer->SetOnGround(false);
		}

		++iter;
	}

	// 敵とステージの当たり判定
	// safety: shared_ptr が null の場合は参照を作らない
	Rect* pEnemyRect = nullptr;
	Rect* pEnemyBatRect = nullptr;

	if (m_pRabbit)
	{
		pEnemyRect = &m_pRabbit->GetRect();
		m_pRabbit->SetOnGround(false); // 毎フレーム最初に false に
	}
	if (m_pBat)
	{
		pEnemyBatRect = &m_pBat->GetRect();
		m_pBat->SetOnGround(false);
	}

	iter = 0;
	const int kMaxIterEnemy = 3; // 速度が大きいなら 4〜6 でも

	if (pEnemyRect)
	{
		while (iter < kMaxIterEnemy && m_pStage->IsCollision(*pEnemyRect, hitTileRect))
		{
			Vec2 push = pEnemyRect->FixPos(hitTileRect); // AABB純押し出し

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
				const float halfH = pEnemyRect->GetH() * 0.5f;
				pEnemyRect->SetY(hitTileRect.GetTop() - halfH);
			}
			else if (push.y > 0.0f) {
				// 天井
				m_pRabbit->SetVelY(0.0f);
				m_pRabbit->SetOnGround(false);
			}

			++iter;
		}
	}

	// Bat 用処理
	iter = 0;
	if (pEnemyBatRect)
	{
		while (iter < kMaxIterEnemy && m_pStage->IsCollision(*pEnemyBatRect, hitTileRect))
		{
			Vec2 push = pEnemyBatRect->FixPos(hitTileRect); // AABB純押し出し

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
				const float halfH = pEnemyBatRect->GetH() * 0.5f;
				pEnemyBatRect->SetY(hitTileRect.GetTop() - halfH);
			}
			else if (push.y > 0.0f) {
				// 天井
				m_pBat->SetVelY(0.0f);
				m_pBat->SetOnGround(false);
			}

			++iter;
		}
	}
	return pointDelta;
}