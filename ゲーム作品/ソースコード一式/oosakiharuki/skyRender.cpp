#include "SkyRender.h"
#include "renderer.h"

void SkyRender::Draw()
{
	Renderer::SetDepthEnable(false);
	ModelRenderer::Draw();
	Renderer::SetDepthEnable(true);
}
