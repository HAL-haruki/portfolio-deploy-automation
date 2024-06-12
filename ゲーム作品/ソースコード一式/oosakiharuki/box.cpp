// Boxオブジェクト [box.cpp]
#include "manager.h"
#include "modelRenderer.h"
#include "shadow.h"
#include "shader.h"

#include "box.h"

void Box::Init()
{
	AddComponent<Shader>()->Load("shader/vertexLightingVS.cso",
		"shader/vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset/model/box.obj");
	AddComponent<Shadow>()->SetSize(8.0f);
}
