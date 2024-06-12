
#include "renderer.h"
#include "field.h"
#include "CreateTexture.h"
#include "shader.h"


void Field::Init()
{
	AddComponent<Shader>()->Load("shader/vertexLightingVS.cso", "shader/vertexLightingPS.cso");

	VERTEX_3D vertex[4];

	vertex[0].Position = DirectX::XMFLOAT3( -50.0f, 0.0f, 50.0f);
	vertex[0].Normal   = DirectX::XMFLOAT3( 0.0f, 1.0f, 0.0f );
	vertex[0].Diffuse  = DirectX::XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[0].TexCoord = DirectX::XMFLOAT2( 0.0f, 0.0f );

	vertex[1].Position = DirectX::XMFLOAT3(50.0f, 0.0f, 50.0f);
	vertex[1].Normal   = DirectX::XMFLOAT3( 0.0f, 1.0f, 0.0f );
	vertex[1].Diffuse  = DirectX::XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[1].TexCoord = DirectX::XMFLOAT2( 10.0f, 0.0f );

	vertex[2].Position = DirectX::XMFLOAT3(-50.0f, 0.0f, -50.0f);
	vertex[2].Normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = DirectX::XMFLOAT2(0.0f, 10.0f);

	vertex[3].Position = DirectX::XMFLOAT3(50.0f, 0.0f, -50.0f);
	vertex[3].Normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = DirectX::XMFLOAT2(10.0f, 10.0f);


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( VERTEX_3D ) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory( &sd, sizeof(sd) );
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer( &bd, &sd, &m_VertexBuffer );


	// テクスチャ読み込み
	CreateTextureFromFile(Renderer::GetDevice(),
						  "asset/texture/field.jpg",
						  &m_Texture);
	assert( m_Texture );

}


void Field::Uninit()
{

	m_VertexBuffer->Release();
	m_Texture->Release();

}


void Field::Draw()
{
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

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);

}
