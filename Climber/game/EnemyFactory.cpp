#include "EnemyFactory.h"
#include "Application.h"
#include "Character.h"
#include "Rabbit.h"
#include "Bat.h"
#include "DxLib.h"

constexpr int kcellSize = 32;
constexpr int khalfCellSize = kcellSize / 2;

EnemyFactory::EnemyFactory(std::shared_ptr<EnemyFactory> ef)
	:m_pEnemyFactory(ef)
{
	m_handles.push_back(LoadGraph("data/Enemy.png"));
	m_handles.push_back(LoadGraph("data/Bat.png"));
}
EnemyFactory::~EnemyFactory()
{
	for (auto& handle : m_handles)
	{
		DeleteGraph(handle);
	}
}

std::shared_ptr<Character> EnemyFactory::Create(int idxX, int idxY, EnemyType enemyType)
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	if (enemyType == EnemyType::none) return nullptr;

	Position2 pos = { idxX * kcellSize + khalfCellSize,
				wsize.h - khalfCellSize - idxY * kcellSize + kcellSize };

	switch (enemyType)
	{
	case EnemyType::usagi:
		m_enemyList.push_back(std::make_shared<Rabbit>(
			m_handles[(int)EnemyType::usagi],
			pos));
		return m_enemyList.back();
	case EnemyType::bat:
		m_enemyList.push_back(std::make_shared<Bat>(
			m_handles[(int)EnemyType::bat],
			pos));
		return m_enemyList.back();
	default:
		return nullptr;
	}
	return nullptr;
}

std::list<std::shared_ptr<Character>>& EnemyFactory::GetEnemyList()
{
	return m_enemyList;
}

void EnemyFactory::Update()
{
	for (auto& enemy : m_enemyList)
	{
		enemy->Update();
	}
}

void EnemyFactory::Draw()
{
	for (auto& enemy : m_enemyList)
	{
		enemy->Draw();
	}
}