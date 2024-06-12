#pragma once


#include "gameObject.h"

class MeshField : public GameObject
{

private:
	ID3D11Buffer*				m_VertexBuffer{};
	ID3D11Buffer*				m_IndexBuffer{};
	ID3D11ShaderResourceView*	m_Texture{};

	VERTEX_3D					m_Vertex[21][21]{};

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;


	float GetHeight( DirectX::XMFLOAT3 Position );

};