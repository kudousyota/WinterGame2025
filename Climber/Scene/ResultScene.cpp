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
	m_fontHandle(-1),
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
	m_fontHandle = CreateFontToHandle("x10y12pxDonguriDuel", 24, -1, -1);
}
void ResultScene::Update(Input& input)
{
	(this->*m_update)(input);
}
void ResultScene::Draw()
{
	(this->*m_draw)();
	DrawStringToHandle(320, 240, "Result Scene", 0xffffff,m_fontHandle);

	// スコア表示
	int score = ResultData::GetScore();
	int kill = ResultData::GetKillCount();	
	char scoreText[64];
	sprintf_s(scoreText,sizeof(scoreText),"SCORE:%d", score);

	DrawStringToHandle(320, 280, scoreText, 0xffffff,m_fontHandle);

	char killText[64];
	sprintf_s(killText, sizeof(killText), "Enemies Defeated: %d", kill);
	DrawStringToHandle(320, 310, killText, 0xffffff,m_fontHandle);

	//操作説明表示
	DrawStringToHandle(320, 350, "Press OK to return to Title", 0xffffff,m_fontHandle);
	
}
