#pragma once
#include "renderer.h"
#include "component.h"


class Cube : public Component
{

private:

	ID3D11Buffer*				m_VertexBuffer{};
	ID3D11Buffer*				m_IndexBuffer{};
	ID3D11ShaderResourceView*	m_Texture{};


public:
	using Component::Component;

	void Init(const char* TextureName);
	void Uninit() override;
	void Draw() override;

};