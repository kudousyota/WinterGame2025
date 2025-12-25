#pragma once

class SceneContoller;
class Input;

class Scene
{
protected:
	SceneContoller& m_controller;
public:
	Scene(SceneContoller& controller);
	//シーンの情報更新
	virtual void Update(Input& input) =0;
	//シーンの描画(シーンの持ち物描画する)
	virtual void Draw() = 0;
};

