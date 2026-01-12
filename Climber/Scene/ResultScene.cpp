#include "ResultScene.h"
#include "DxLib.h"
#include "../System/Input.h"
#include "TitleScene.h"
#include "SceneContoller.h"
#include "../System/ResultData.h"

ResultScene::ResultScene(SceneContoller& controller) :
	Scene(controller)
{
}
void ResultScene::Init()
{
}
void ResultScene::Update(Input& input)
{
	if (input.IsTriggered("ok"))
	{
		m_controller.ChangeScene(std::make_shared<TitleScene>(m_controller));
		return;
	}
}
void ResultScene::Draw()
{

	DrawString(320, 240, "Result Scene", 0xffffff);

	// スコア表示
	int score = ResultData::GetScore();
	char scoreText[64];
	sprintf_s(scoreText, "Score: %d", score);
	DrawString(320, 280, scoreText, 0xffffff);

	//操作説明表示
	DrawString(320, 320, "Press OK to return to Title", 0xffffff);

}
