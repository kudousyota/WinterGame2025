#include "Application.h"
#include "DxLib.h"
#include <cassert>
#include "input.h"
#include "SceneContoller.h"
#include "TitleScene.h"

constexpr int default_window_width = 640;	// デフォルトウィンドウ幅
constexpr int default_window_height = 480;	// デフォルトウィンドウ高
constexpr int default_color_bit = 32;	// デフォルトカラービット

Application::Application() :
	windowSize_{ default_window_width,default_window_height }
{
}

Application::~Application()
{
}

Application& Application::GetInstance()
{
	// staticなのでメモリの場所が一つで確定→これ一つしか実態がない→シングルトンクラス
	static Application instance;
	return instance;	// インスタンスの参照を返す→Applicationクラスの場所を返す
	// staticで一つしか存在しないものの参照を返すので複数になることはない
}

bool Application::Init()
{
	SetWindowSize(windowSize_.w, windowSize_.h);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)
	{
		return false;
	}
	return true;
}

void Application::Run()
{
	SetDrawScreen(DX_SCREEN_BACK);
	Input input;
	SceneContoller controller;
	controller.ChangeScene(std::make_shared<TitleScene>(controller));
	while (ProcessMessage() != -1 && !requestedExit_)
	{
		ClearDrawScreen();
		input.Update();	// 入力情報の更新
		controller.Update(input);
		controller.Draw();

		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}

		ScreenFlip();
	}
}

void Application::Terminate()
{
	DxLib_End();
}

const Size& Application::GetWindowSize() const
{
	return windowSize_;
}

void Application::RequestExit()
{
	requestedExit_ = true;
}
