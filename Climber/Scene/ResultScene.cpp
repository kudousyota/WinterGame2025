#include <algorithm>
#include <cmath>
#include "ResultScene.h"
#include "DxLib.h"
#include "../System/Input.h"
#include "TitleScene.h"
#include "SceneContoller.h"
#include "../System/Application.h"
#include "../System/ResultData.h"
#include "../System/Vec2.h"
#include "../System/SoundManager.h"

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
	// スコアカウントアップ演出
	if (m_displayScore < m_finalScore)
	{
		m_scoreAnimTime += 0.05f;

		// 1以上にいかないようにクランプ
		float t = (1.0f, m_scoreAnimTime);

		m_displayScore = static_cast<int>(
			std::lerp(0.0f, static_cast<float>(m_finalScore), t)
			);
	}



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
	DrawRotaGraph(wsize.w / 2, wsize.h / 3, 0.7f, 0.0f, m_ResultLogoH, true);
}

void ResultScene::FadeDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	DrawRotaGraph(wsize.w / 2, wsize.h / 2, 4.0f, 0.0f, m_ResultH, true);
	DrawRotaGraph(wsize.w / 2, wsize.h / 3, 0.7f, 0.0f, m_ResultLogoH, true);
	//値の範囲を一旦0.0~1.0fにしておくといろいろと扱いやすい
	auto rate = static_cast<float>(m_frame) / static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * rate);
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}


ResultScene::ResultScene(SceneContoller& controller) :
	m_draw(0),
	m_fontHandle(-1),
	m_displayScore(0),
	m_finalScore(0),
	m_scoreAnimTime(0),
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
{																	//枠取りできる							
	m_fontHandle = CreateFontToHandle("x10y12pxDonguriDuel", 40, 6, DX_FONTTYPE_ANTIALIASING_EDGE);
	//色は描画時に指定
	const int white = GetColor(255,255,255);
	m_finalScore = ResultData:: GetScore();

	m_displayScore = 0;
	m_scoreAnimTime = 0.0f;
}
void ResultScene::Update(Input& input)
{
	(this->*m_update)(input);
}
void ResultScene::Draw()
{
	const int white = GetColor(255, 255, 255);
	const int Cyan = GetColor(0, 255, 255);
	const int Color = GetColor(224, 255, 255);
	const auto& wsize = Application::GetInstance().GetWindowSize();
	(this->*m_draw)();
	//DrawStringToHandle(320, 240, "Result Scene", 00000000,m_fontHandle);

	// スコア表示
	int score = ResultData::GetScore();
	int kill = ResultData::GetKillCount();	
	char scoreText[64];
	sprintf_s(scoreText,sizeof(scoreText),"SCORE:%d", m_displayScore);

	DrawStringToHandle(wsize.w / 2.3, wsize.h/2, scoreText, Cyan,m_fontHandle);

	char killText[64];
	sprintf_s(killText, sizeof(killText), "Enemies Defeated: %d", kill);
	DrawStringToHandle(wsize.w/3, wsize.h /1.5, killText, Cyan,m_fontHandle);

	//点滅頻度
	const int intervar = 650;
	int now = GetNowCount();
	bool visible = (now / intervar) % 2;
	if (visible)
	{
		//操作説明表示
		DrawStringToHandle(470, 580, "Press A to Title", Color, m_fontHandle);
	}

}
