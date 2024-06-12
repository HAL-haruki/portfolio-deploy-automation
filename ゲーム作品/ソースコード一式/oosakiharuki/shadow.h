#pragma once
#include "component.h"


class Shadow : public Component
{

private:

	ID3D11Buffer*				m_VertexBuffer{};
	ID3D11ShaderResourceView*	m_Texture{};
	DirectX::XMFLOAT3 m_Position{};

	float m_Size = 1.0f;

public:
	using Component::Component;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetSize(float Size) { m_Size = Size; }

};
#pragma once
