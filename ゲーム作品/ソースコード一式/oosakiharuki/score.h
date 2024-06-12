#pragma once

#include "gameObject.h"

class Score : public GameObject
{

private:
	ID3D11Buffer*               m_VertexBuffer{};
	ID3D11ShaderResourceView*   m_Texture{};

	int m_Count;

public:
	void Init();
	void Uninit();
	void Draw();
	void SetCount(int count) { m_Count = count; }
	void AddCount(int Count) { m_Count += Count; }
};
