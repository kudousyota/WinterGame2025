#pragma once

#include "Geometry.h"

/// <summary>
/// アプリケーション全体を管理する
/// シングルトンクラス
/// </summary>
class Application
{
private:
	Size windowSize_;

	Application();	// privateにすることでシングルトンクラスにできる
	Application(const Application& app) = delete;	// コピーコンストラクタもprivateにして複製を防ぐ
	void operator=(const Application& app) = delete;	// = deleteをつけると何をしてもこれを使えなくなる

	bool requestedExit_ = false;	// ゲームの終了がリクエストされた

public:
	~Application();
	static Application& GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <returns>初期化成功:true / 初期化失敗:false</returns>
	bool Init();

	/// <summary>
	/// アプリケーションをスタートする
	/// ゲームループを内包する
	/// </summary>
	void Run();

	/// <summary>
	/// アプリケーション終了処理
	/// </summary>
	void Terminate();

	/// <summary>
	/// ウィンドウサイズを返す
	/// </summary>
	/// <returns>ウィンドウサイズ</returns>
	const Size& GetWindowSize() const;

	/// <summary>
	/// アプリケーションを終了する命令を飛ばす
	/// </summary>
	void RequestExit();
};

