#include "Rabbit.h"
#include "Dxlib.h"
#include "Camera.h"
#include "Player.h"
namespace
{
	constexpr float kGravity = 0.3f;
	//constexpr float kGroundY = 1520.0f;
}

Rabbit::Rabbit() :
	m_vel(0.0f),
	m_Handle(-1),
	m_cutX(0),
	m_cutY(0),
	m_cutW(0),
	m_cutH(0),
	m_switchSpeed(0.0f),
	m_pos(0, 0),
	m_isLeft(false),
	m_speed(0),
	m_frameCount(0),
	m_ownsHandle(true)
{
	// 自前で生成する場合は自身でロード・所有
	m_Handle = LoadGraph("data/Enemy.png");
	m_pCamera = std::make_shared<Camera>();
}

Rabbit::Rabbit(int handle, const Position2& pos) :
	m_vel(0.0f),
	m_Handle(handle),
	m_cutX(0),
	m_cutY(0),
	m_cutW(0),
	m_cutH(0),
	m_switchSpeed(0.0f),
	m_pos(pos.x, pos.y),
	m_isLeft(false),
	m_speed(0),
	m_frameCount(0),
	m_ownsHandle(false)
{
	// Factory から渡されたハンドルは自分で削除しない
	m_pCamera = std::make_shared<Camera>();
	// 初期当たり判定は位置に合わせてセットしておく
	m_rect.Init(m_pos.x, m_pos.y, 50.0f, 50.0f);
}

Rabbit::~Rabbit()
{
	// 自分でロードしたハンドルだけ削除する（Factory 管理のハンドルは削除しない）
	if (m_ownsHandle && m_Handle != -1)
	{
		DeleteGraph(m_Handle);
	}
}
void Rabbit::Init()
{
	m_rect.Init(300.0f, 3030.0f, 50.0f, 50.0f);
	m_vel = 0.0f;
	//アニメーション
	m_cutW = 34;
	m_cutH = 45;
	m_speed = -1;

}
void Rabbit::Update(const Player& player)
{
	m_pos = { m_rect.GetX(),m_rect.GetY() };
	//アニメーションの更新
	m_frameCount++;
	if (m_frameCount >= 5)
	{
		m_frameCount = 0;
		m_switchSpeed++;
		//横にずらす
		if (m_switchSpeed >= 5)
		{
			m_switchSpeed = 0;
		}
		m_cutX = m_switchSpeed * m_cutW;//切り取りいち
		m_cutY = 0;
	}
	//重力
	m_vel += kGravity;

	// 速度を位置へ反映
	m_rect.SetX(m_rect.GetX() + static_cast<float>(m_speed));
	m_rect.SetY(m_rect.GetY() + m_vel);

}
void Rabbit::Draw(const Camera& camera)
{
	const Vec2 cameraOffset = camera.GetCameraOffset();

	//中心座標
	const int centerX = static_cast<int>(m_rect.GetX() + cameraOffset.x);
	const int centerY = static_cast<int>(m_rect.GetY() + cameraOffset.y);

	//左上、右上、左下、右下の座標計算
	const int halfW = static_cast<int>(m_rect.GetW() * 0.5f);
	const int halfH = static_cast<int>(m_rect.GetH() * 0.5f);

	const int left = centerX - halfW;
	const int right = centerX + halfW;
	const int Top = centerY - halfH;
	const int Bottom = centerY + halfH;



	DrawRectRotaGraph(
		centerX, centerY,            // 画面の中心位置
		m_cutX, m_cutY,              // 切り抜き開始位置
		m_cutW, m_cutH,              // 切り抜きサイズ（例：32×32）
		1.0f,                        // 拡大率
		0.0f,                        // 回転角度
		m_Handle,                    // 画像ハンドル
		true,                         // 透過あり
		m_isLeft					  // 左右反転
	);


#ifdef _DEBUG
	//当たり判定の枠
	DrawBox(
		left, Top,
		right, Bottom,
		GetColor(255, 0, 0),
		false
	);
#endif

}

bool Rabbit::isHit(const Player& player)
{
	//ここに当たった時の処理

	return m_rect.IsHit(player.GetRect());
}