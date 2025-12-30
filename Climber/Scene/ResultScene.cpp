#include "ResultScene.h"
#include "DxLib.h"
#include "Input.h"
#include "TitleScene.h"
#include "SceneContoller.h"

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
}
