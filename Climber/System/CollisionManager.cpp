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
int CollisionManager::CheckCollisions(std::shared_ptr<Player> m_pPlayer, std::vector<std::shared_ptr<Rabbit>> m_pRabbits, std::vector<std::shared_ptr<Bat>>m_pBats, std::shared_ptr<Stage> m_pStage)
{
	int pointDeleta = 0;

	if (!m_pPlayer || !m_pStage)
	{
		return pointDeleta;
	}
	Rect hitTileRect;
	//ウサギと当たった時の処理
	for (auto& rabbit : m_pRabbits)
	{
		if (!rabbit)continue;

		if (m_pPlayer->IsHit(*rabbit))
		{
			Vec2 push = m_pPlayer->FixPos(*rabbit);

			if (!rabbit->IsDead)
			{
				if (m_pPlayer->IsHighJumpUnlock())
				{
					//ウサギが死んだとき
					rabbit->OnDead();
					pointDeleta += 50;
				}
				else
				{
					if (!m_pPlayer->Isinvincible())
					{

						pointDeleta -= 10;
						//無敵時間
						m_pPlayer->StartInvincible(120);

						Rect& playerRect = m_pPlayer->GetRect();
						Rect& enemyRect = rabbit->GetRect();

						//敵と当たった時のノックバック
						if (playerRect.GetX() < enemyRect.GetX())
						{
							playerRect.SetX(playerRect.GetX() - knockbackPpwerX);
							playerRect.SetY(playerRect.GetY() + knockbackPpwerY);
						}
						else
						{
							playerRect.SetX(playerRect.GetX() + knockbackPpwerX);
							playerRect.SetY(playerRect.GetY() + knockbackPpwerY);
						}

						m_pPlayer->SetVelY(knockbackPpwerY);
						m_pPlayer->SetOnGround(false);
					}
				}
			}
		}
	}

	//コウモリと当たった時の処理
	for (auto& bat : m_pBats)
	{
		if (!bat)continue;
		if (m_pPlayer->IsHit(*bat))
		{
			Vec2 push = m_pPlayer->FixPos(*bat);

			if (!bat->IsDead)
			{
				if (m_pPlayer->IsHighJumpUnlock())
				{
					bat->OnDead();
					pointDeleta += 50;
				}
				else
				{
					if (!m_pPlayer->Isinvincible())
					{
						pointDeleta -= 10;
						m_pPlayer->StartInvincible(120);

						Rect& playerRect = m_pPlayer->GetRect();
						Rect& batRect = bat->GetRect();

						if (playerRect.GetX() < batRect.GetX())
						{
							playerRect.SetX(playerRect.GetX() - knockbackPpwerX);

						}
						else
						{
							playerRect.SetX(playerRect.GetX() + knockbackPpwerX);
						}
						m_pPlayer->SetVelY(knockbackPpwerY);
						m_pPlayer->SetOnGround(false);

					}
				}
			}
		}
	}
	//プレイヤーとステージの当たり判定
	Rect& playerRect = m_pPlayer->GetRect();
	m_pPlayer->SetOnGround(false);

	int iter = -0;
	const int kMaxIter = 3;

	while (iter < kMaxIter && m_pStage->IsCollision(playerRect, hitTileRect))
	{
		Vec2 push = playerRect.FixPos(hitTileRect);

		if (push.y < 0.0f)
		{
			float halfH = playerRect.GetH() * 0.5;
			playerRect.SetY(hitTileRect.GetTop() - halfH);
			m_pPlayer->SetVelY(0.0f);
			m_pPlayer->SetOnGround(true);
		}
		else if (push.y > 0.0f)
		{
			int tx = hitTileRect.GetX() / m_pStage->GetChipW();
			int ty = hitTileRect.GetY() / m_pStage->GetChipH();

			if (m_pStage->GetData(tx, ty) != 0)
			{
				m_pStage->SetTile(tx, ty, 0);

				if (!m_pPlayer->IsHighJumpActive())
					m_pPlayer->SetVelY(0.0f);

				m_pPlayer->TileBroke();
			}

			m_pPlayer->SetOnGround(false);
		}

		++iter;
	}


	for (auto& bat : m_pBats)
	{
		if (!bat)continue;

		Rect& rect = bat->GetRect();
		bat->SetOnGround(false);

		iter = 0;
		while (iter < 3 && m_pStage->IsCollision(rect, hitTileRect))
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
	return pointDeleta;
}
