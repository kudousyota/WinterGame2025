#pragma once
#include <memory>
#include <list>
#include <vector>

class Character;
class Player;

enum class EnemyType
{
	none,
	usagi,
	bat
};

class EnemyFactory
{
public:
	EnemyFactory(std::shared_ptr<EnemyFactory>ef);
	~EnemyFactory();

	// 敵生成（共通基底 Character を返す）
	std::shared_ptr<Character> Create(int idxX, int idxY, EnemyType enemyType);

	// 敵リストの取得（Character の shared_ptr リストに変更）
	std::list<std::shared_ptr<Character>>& GetEnemyList();

	void Update();
	void Draw();
private:
	//あらかじめ敵の表示に必要な画像ハンドルをロードして保持しておく
	std::vector<int> m_handles;
	std::shared_ptr<EnemyFactory>m_pEnemyFactory;
	std::list<std::shared_ptr<Character>>m_enemyList;
};