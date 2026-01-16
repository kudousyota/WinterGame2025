#include "ResultScene.h"
#include "DxLib.h"
#include "../System/Input.h"
#include "TitleScene.h"
#include "SceneContoller.h"
#include "../System/Application.h"
#include "../System/ResultData.h"

constexpr int fade_interval = 60;

void ResultScene::FadeInUpdate(Input& input)
{
	// フェードイン中でもエンター("ok")でフェードアウト（シーン切替）を開始できるようにする
	if (input.IsTriggered("ok"))
	{
		m_update = &ResultScene::FadeOutUpdate;
		m_draw = &ResultScene::FadeDraw;
		m_frame = 0;	// フェードアウトの最初
		return;
	}


	if (m_frame-- <= 0)
	{
		m_update = &ResultScene::NormalUpdate;
		m_draw = &ResultScene::NormalDraw;
		return;
	}
}

void ResultScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("ok"))
	{
		m_update = &ResultScene::FadeOutUpdate;
		m_draw = &ResultScene::FadeDraw;
		m_frame = 0;	// フェードアウトの最初
		return;
	}
}

void ResultScene::FadeOutUpdate(Input&)
{
	if (m_frame ++ >= fade_interval)
	{
		m_controller.ChangeScene(std::make_shared<TitleScene>(m_controller));
		return;
	}
}
void ResultScene::NormalDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	DrawRotaGraph(wsize.w / 2, wsize.h / 2, 4.0f, 0.0f, m_ResultH, true);
	DrawRotaGraph(wsize.w / 2, wsize.h / 4, 0.25f, 0.0f, m_ResultLogoH, true);
}

void ResultScene::FadeDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	DrawRotaGraph(wsize.w / 2, wsize.h / 2, 4.0f, 0.0f, m_ResultH, true);
	DrawRotaGraph(wsize.w / 2, wsize.h / 4, 0.25f, 0.0f, m_ResultLogoH, true);
	//値の範囲を一旦0.0~1.0fにしておくといろいろと扱いやすい
	auto rate = static_cast<float>(m_frame) / static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * rate);
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}


ResultScene::ResultScene(SceneContoller& controller) :
	m_draw(0),
	Scene(controller)
{
	m_ResultH = LoadGraph("data/bg.png");
	m_ResultLogoH = LoadGraph("data/game_result.png");
	m_update = &ResultScene::FadeInUpdate;
	m_draw = &ResultScene::FadeDraw;
	m_frame = fade_interval;
}
ResultScene::~ResultScene()
{
	//何もしない
}

void ResultScene::Init()
{
}
void ResultScene::Update(Input& input)
{
	(this->*m_update)(input);
}
void ResultScene::Draw()
{
	(this->*m_draw)();
	DrawString(320, 240, "Result Scene", 0xffffff);

	// スコア表示
	int score = ResultData::GetScore();
	int kill = ResultData::GetKillCount();	
	char scoreText[64];
	char killText[64];
	sprintf_s(scoreText, "Score: %d", score);
	DrawString(320, 280, scoreText, 0xffffff);
	sprintf_s(killText, "Enemies Defeated: %d", kill);
	DrawString(320, 310, killText, 0xffffff);

	//操作説明表示
	DrawString(320, 320, "Press OK to return to Title", 0xffffff);
	
}
