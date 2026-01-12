#include "DxLib.h"
#include "System/Game.h"
#include"System/Bg.h"
#include "System/Input.h"
#include <memory> //シェアードポイント

#include "Scene/SceneMain.h"
#include "Scene/TitleScene.h"
#include "Scene/SceneContoller.h"
//定数定義
namespace
{
	//画面サイズ
	constexpr int kScreenWidth = 1280;
	constexpr int kScreenHeight = 720;
}


// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//ウィンドウモード設定
	ChangeWindowMode(true);
	//タイトル変更
	SetMainWindowText("ゲーム名");
	//画面のサイズ変更
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorBitNum);
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	//描画対象をバックバッファに変更
	SetDrawScreen(DX_SCREEN_BACK);

	// Input 作成（毎フレーム更新してシーンに渡す）
	Input input;

	// シーンコントローラを作成（SceneMain のコンストラクタは参照を要求する）
	SceneContoller controller;

	// SceneMain を controller を渡して生成
	auto pScene = std::make_shared<TitleScene>(controller);
	pScene->Init();

	// コントローラに現在のシーンを登録（以後は controller 経由で Update/Draw）
	controller.ResetScene(pScene);

	while (ProcessMessage() != -1)
	{
		//このフレームの開始時間を取得
		LONGLONG start = GetNowHiPerformanceCount();

		//前のフレームに描画した内容をクリアする
		ClearDrawScreen();

		// 入力更新（必ずフレーム先頭で更新）
		input.Update();

		// シーン処理をコントローラ経由で呼ぶ（直接 pScene->Update を呼ばない）
		controller.Update(input);
		controller.Draw();

		//escキーを押すとゲームを強制終了
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}
		//描画した内容を画面に反映する
		ScreenFlip();

		//フレームレート６０に固定
		while (GetNowHiPerformanceCount() - start < 16667)
		{

		}

	}

	//メモリの解放

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}