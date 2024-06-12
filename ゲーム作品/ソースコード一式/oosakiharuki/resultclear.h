#pragma once

#include "scene.h"

class ResultClear : public Scene
{
private:
	class Transition* m_Transition{};
public:
	void Init() override;
	void Update() override;

};

