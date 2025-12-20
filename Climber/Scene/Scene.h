#pragma once
class ScemeComtroller;
class Scene
{
protected:
	ScemeComtroller& m_controller;
public:
	Scene(ScemeComtroller& controller);
	//シーンの情報更新
	virtual void Update() =0;
	//シーンの描画(シーンの持ち物描画する)
	virtual void Draw() = 0;
};

