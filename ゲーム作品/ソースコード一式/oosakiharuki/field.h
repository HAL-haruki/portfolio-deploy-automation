#pragma once
#include "gameObject.h"


class Field : public GameObject
{

private:

	ID3D11Buffer*				m_VertexBuffer{};
	ID3D11ShaderResourceView*	m_Texture{};

public:
	void Init() override;
	void Uninit() override;
	void Draw() override;

};
