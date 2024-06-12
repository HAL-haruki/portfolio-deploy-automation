#pragma once

#include "scene.h"

class Game : public Scene
{
private:
	class Audio*        m_BGM{};
	class Transition* m_Transition{};
	bool m_Goal = false;
	bool m_PlayerDead = false;
	bool m_Clear = false;
	bool m_screen;
	bool m_isOnce;
	bool m_isOnce2;
public:
	void Init() override;
	void Uninit() override; 
	void Update() override;

};
