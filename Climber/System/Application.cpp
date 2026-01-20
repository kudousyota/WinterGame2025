#include "Application.h"
#include "EffekseerForDXLib.h"
#include "DxLib.h"
#include <cassert>
#include "input.h"
#include "../Scene/SceneContoller.h"
#include "../Scene/TitleScene.h"

constexpr int default_window_width = 1280;	// デフォルトウィンドウ幅
constexpr int default_window_height = 720;	// デフォルトウィンドウ高
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

	//エフェクシア
	//DirectX11を使用するようにする
	//Effekseerを必ず設定する
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	//effekseerを初期化する
	//引数に画面に表示する最大パーティクル数を設定する
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
		return -1;
	}
	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
		// Effekseerを使用する場合は必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DXライブラリのデバイスロストした時のコールバックを設定する。
	// ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
	// ただし、DirectX11を使用する場合は実行する必要はない。
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// Effekseerに2D描画の設定をする。
	Effekseer_Set2DSetting(windowSize_.w, windowSize_.h);

	// Zバッファを有効にする。
	// Effekseerを使用する場合、2DゲームでもZバッファを使用する。
	SetUseZBuffer3D(TRUE);

	// Zバッファへの書き込みを有効にする。
	// Effekseerを使用する場合、2DゲームでもZバッファを使用する。
	SetWriteZBuffer3D(TRUE);
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
	Effkseer_End();
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
