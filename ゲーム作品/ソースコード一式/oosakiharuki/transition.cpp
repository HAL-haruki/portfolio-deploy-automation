#include "transition.h"
#include "renderer.h"
#include "shader.h"
#include "sprite.h"

void Transition::Init()
{
	AddComponent<Shader>()->Load("shader/unlitTextureVS.cso",
		"shader/unlitTexturePS.cso");
	m_Sprite = AddComponent<Sprite>();
	m_Sprite->Init(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	MATERIAL material{};
	material.Diffuse = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	material.TextureEnable = FALSE;
	m_Sprite->SetMaterial(material);
}

void Transition::Update()
{
	switch (m_State)
	{
	case State::Stop:
		break;

	case State::In:
		m_Alpha -= 2.0f / 60.f;
		if (m_Alpha < 0.0f)
		{
			m_Alpha = 0.0f;
			m_State = State::Stop;
		}
		break;

	case State::Out:
		m_Alpha += 2.0f / 60.f;
		if (m_Alpha > 1.0f)
		{
			m_Alpha = 1.0f;
			m_State = State::Finish;
		}
		break;

	case State::Finish:
		break;

	default:
		break;
	}
	MATERIAL material{};
	material.Diffuse = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, m_Alpha);
	material.TextureEnable = FALSE;
	m_Sprite->SetMaterial(material);
}
