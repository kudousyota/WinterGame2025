#pragma once
#include "Scene.h"

class TitleScene : public Scene //Sceneを継承したTitleSceneクラス
{
public:
	TitleScene(SceneContoller& controller); //コンストラクタ
	~TitleScene(); //デストラクタ
	void Init() override;//初期化処理
	void Update(Input& input) override; //更新処理
	void Draw() override; //描画処理
private:
	//タイトルの画像
	int m_titleH;
	//タイトルのロゴ画像
	int m_titleLogoH;
	//エフェクとのハンドル
	int m_effectHandle;
	//再生用にハンドル
	int m_effectPlayingHandle;
	//経過フレーム
	int m_frame;



	// メンバ関数ポインタを使用して内部状態を書き換える
	// update関数とdraw関数を、現在のTitleSceneの状態に合わせて
	// 切り替えていく

	//フェードインupdate
	void FadeInUpdate(Input& input);
	//通常update
	void NormalUpdate(Input&);
	//フェードアウトupdate
	void FadeOutUpdate(Input&);
	using UpdateFunc_t = void(TitleScene::*)(Input&);
	//Updateメンバ関数を代入
	UpdateFunc_t m_update;

	void NormalDraw();	// 通常Draw
	void FadeDraw();	// フェードDraw
	using DrawFunc_t = void(TitleScene::*)();
	DrawFunc_t m_draw;	// Drawメンバ関数を代入できるメンバ関数ポインタ


};

