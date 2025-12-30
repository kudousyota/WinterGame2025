#pragma once
#include "Scene.h"


class ResultScene :public Scene
{
public:
	ResultScene(SceneContoller& controller);
	void Init()override;
	void Update(Input& input)override;
	void Draw()override;
};

