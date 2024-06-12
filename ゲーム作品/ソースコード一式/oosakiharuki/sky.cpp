#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "SkyRender.h"
#include "sky.h"
#include "camera.h"
#include "shader.h"

void Sky::Init()
{

	AddComponent<Shader>()->Load("shader/unlitTextureVS.cso", "shader/unlitTexturePS.cso");
	AddComponent<SkyRender>()->Load("asset/model/sky.obj");
	m_Scale = DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f);

}


void Sky::Update()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();

	DirectX::XMFLOAT3 cameraPosition = camera->GetPosition();
	m_Position = cameraPosition;
}
