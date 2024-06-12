#include "manager.h"
#include "modelRenderer.h"
#include "shadow.h"
#include "shader.h"

#include "cylinder.h"

void Cylinder::Init()
{
	AddComponent<Shader>()->Load("shader/vertexLightingVS.cso",
		"shader/vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset/model/cylinder.obj");
	AddComponent<Shadow>()->SetSize(5.0f);
}
