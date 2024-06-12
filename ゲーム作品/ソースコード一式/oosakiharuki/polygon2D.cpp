
#include "renderer.h"
#include "polygon2D.h"
#include "shader.h"
#include "sprite.h"


void Polygon2D::Init()
{
	AddComponent<Shader>()->Load("shader/unlitTextureVS.cso", "shader/unlitTexturePS.cso");

	AddComponent<Sprite>()->Init(0, 0, 200, 200, "asset/texture/grass.jpg");
}
