#include "TitleScene.h"
#include "DxLib.h"
#include "Input.h"
#include "SceneMain.h"
#include "SceneContoller.h"
#include "Application.h"

constexpr int fade_interval = 60;

void TitleScene::FadeInUpdate(Input& input)
{
	// フェードイン中でもエンター("ok")でフェードアウト（シーン切替）を開始できるようにする
	if (input.IsTriggered("ok"))
	{
		m_update = &TitleScene::FadeOutUpdate;
		m_draw = &TitleScene::FadeDraw;
		m_frame = 0;	// フェードアウトの最初
		return;
	}


	if (m_frame-- <= 0)
	{
		m_update = &TitleScene::NormalUpdate;
		m_draw = &TitleScene::NormalDraw;
		return;
	}
}

void TitleScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("ok"))
	{
		m_update = &TitleScene::FadeOutUpdate;
		m_draw = &TitleScene::FadeDraw;
		m_frame = 0;	// フェードアウトの最初
		return;
	}
}

void TitleScene::FadeOutUpdate(Input&)
{
	if (m_frame ++ >= fade_interval)
	{
		m_controller.ChangeScene(std::make_shared<SceneMain>(m_controller));
		return;
	}
}

void TitleScene::NormalDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	DrawRotaGraph(wsize.w / 2, wsize.h / 2, 1.0f, 0.0f, m_titleH, true);
	DrawRotaGraph(wsize.w / 2, wsize.h / 2, 0.75f, 0.0f, m_titleLogoH, true);
}

void TitleScene::FadeDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	DrawRotaGraph(wsize.w / 2, wsize.h / 2, 1.0f, 0.0f, m_titleH, true);
	DrawRotaGraph(wsize.w / 2, wsize.h / 2, 0.75f, 0.0f, m_titleLogoH, true);
	// 値の範囲をいったん0.0~1.0にしておくといろいろと扱いやすくなります
	auto rate = static_cast<float>(m_frame) / static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * rate);
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);	// 画面全体に黒フィルムをかける
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	// ブレンドしない
}

TitleScene::TitleScene(SceneContoller& controller) : Scene(controller)
{
	//m_titleH = LoadGraph("data/title_bg.png");
	m_titleLogoH = LoadGraph("data/game_title.png");
	m_update = &TitleScene::FadeInUpdate;
	m_draw = &TitleScene::FadeDraw;
	m_frame = fade_interval;
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
}

void TitleScene::Update(Input& input)
{
	(this->*m_update)(input);
}

void TitleScene::Draw()
{
	(this->*m_draw)();
}