
#include "renderer.h"
#include "shadow.h"
#include "CreateTexture.h"
#include "gameObject.h"
#include "manager.h"
#include "scene.h"
#include "box.h"
#include "cylinder.h"


void Shadow::Init()
{

	VERTEX_3D vertex[4];

	vertex[0].Position = DirectX::XMFLOAT3(-1.0f, 0.01f, 1.0f);
	vertex[0].Normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = DirectX::XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = DirectX::XMFLOAT3(1.0f, 0.01f, 1.0f);
	vertex[1].Normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = DirectX::XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = DirectX::XMFLOAT3(-1.0f, 0.01f, -1.0f);
	vertex[2].Normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = DirectX::XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = DirectX::XMFLOAT3(1.0f, 0.01f, -1.0f);
	vertex[3].Normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = DirectX::XMFLOAT2(1.0f, 1.0f);


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);


	// テクスチャ読み込み
	CreateTextureFromFile(Renderer::GetDevice(),
		"asset/texture/shadow.png",
		&m_Texture);
	assert(m_Texture);

}


void Shadow::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

}
void Shadow::Update()
{
	//接地
	float groundHeight = 0.0f;

	Scene* scene = Manager::GetScene();
	{
		std::vector<Cylinder*> cylinderList = scene->GetGameObjects<Cylinder>();
		for (Cylinder* cylinder : cylinderList)
		{
			if (cylinder == m_GameObject)
				continue;
			DirectX::XMFLOAT3 position = cylinder->GetPosition();
			DirectX::XMFLOAT3 scale = cylinder->GetScale();
			DirectX::XMFLOAT3 direction =
				DirectX::XMFLOAT3(m_Position.x - position.x,
					0.0f, m_Position.z - position.z);
			float length;
			DirectX::XMStoreFloat(&length,
				DirectX::XMVector3Length(DirectX::XMLoadFloat3(&direction)));
			if (length < scale.x)
			{
				groundHeight = position.y + scale.y;
				break;
			}
		}
	}
	{
		std::vector<Box*> boxList = scene->GetGameObjects<Box>();
		for (Box* box : boxList)
		{
			if (box == m_GameObject)
				continue;
			DirectX::XMFLOAT3 position = box->GetPosition();
			DirectX::XMFLOAT3 scale = box->GetScale();
			if (position.x - scale.x - 0.5f < m_Position.x &&
				m_Position.x < position.x + scale.x + 0.5f &&
				position.z - scale.z - 0.5f < m_Position.z &&
				m_Position.z < position.z + scale.z + 0.5f)
			{
				groundHeight = position.y + scale.y * 2.0f;
				break;
			}
		}
	}
	m_Position = m_GameObject->GetPosition();
	m_Position.y = groundHeight;
}


void Shadow::Draw()
{
	DirectX::XMFLOAT3 position = m_GameObject->GetPosition();
	position.y = 0.0f;

	//// マトリクス設定
	//DirectX::XMFLOAT4X4 world;
	//DirectX::XMMATRIX scale, rot, trans;
	//scale = DirectX::XMMatrixScaling(m_Size, m_Size, m_Size);
	////rot = DirectX::XMMatrixRotationRollPitchYaw(position.x, position.y, position.z);
	//trans = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	
	// マトリクス設定
	DirectX::XMFLOAT4X4 world;
	DirectX::XMMATRIX scale, trans;
	scale = DirectX::XMMatrixScaling(m_Size, m_Size, m_Size);
	trans = DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	DirectX::XMStoreFloat4x4(&world, scale * /*rot **/ trans);

	Renderer::SetWorldMatrix(&world);
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	Renderer::SetDepthEnable(false);
	
	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);

	Renderer::SetDepthEnable(true);
}
