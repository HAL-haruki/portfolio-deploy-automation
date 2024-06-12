#include "manager.h"
#include "renderer.h"
#include "shader.h"



void Shader::Load(const char* VertexShader, const char* PixelShader)
{

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, VertexShader);

	Renderer::CreatePixelShader(&m_PixelShader, PixelShader);
}

void Shader::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}


void Shader::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, nullptr, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, nullptr, 0);

}
