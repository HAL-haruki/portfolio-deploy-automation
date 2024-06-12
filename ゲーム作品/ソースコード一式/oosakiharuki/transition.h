#pragma once
#include "gameObject.h"

class Transition : public GameObject
{
public:
	enum class State
	{
		Stop,
		In,
		Out,
		Finish
	};

private:
	float m_Alpha = 0.0f;
	State m_State = State::Stop;
	class Sprite* m_Sprite{};

public:
	void Init() override;
	void Update() override;

	State GetState() { return m_State; }

	void FadeIn()
	{
		m_Alpha = 1.0f;
		m_State = State::In;
	}
	void FadeOut()
	{
		m_Alpha = 0.0f;
		m_State = State::Out;
	}
};
