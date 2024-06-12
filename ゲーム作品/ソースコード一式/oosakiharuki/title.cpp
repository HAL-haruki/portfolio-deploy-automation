
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "game.h"
#include "shader.h"
#include "sprite.h"
#include "transition.h"

#include "title.h"

void Title::Init()
{
	GameObject* titleLogo = AddGameObject<GameObject>(3);
	titleLogo->AddComponent<Shader>()->Load("shader/unlitTextureVS.cso", "shader/unlitTexturePS.cso");
	titleLogo->AddComponent<Sprite>()->Init(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, "asset/texture/title1.png");
	//titleLogo->AddComponent<Sprite>()->Init(200, 200, 900, 200, "asset/texture/flash.png");
	titleLogo->AddComponent<Sprite>()->Init(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, "asset/texture/flash.png");
	
	
	m_Transition = AddGameObject<Transition>(4);
	m_Transition->FadeIn();

}


void Title::Update()
{
	Particle tenp;
	tenp.game = AddGameObject<GameObject>(3);
	tenp.game->SetPosition(DirectX::XMFLOAT3(SCREEN_WIDTH* 0.7f, SCREEN_HEIGHT * 0.9f, 0.0f));
	tenp.game->AddComponent<Shader>()->Load("shader/unlitTextureVS.cso", "shader/unlitTexturePS.cso");
	tenp.game->AddComponent<Sprite>()->Init(SCREEN_WIDTH* 0.7f, SCREEN_HEIGHT * 0.9f, 100.0f, 100.0f, "asset/texture/en.png");
	tenp.m_vel.y = 20;
	tenp.m_vel.x = rand() % 5 - 2;
	m_particle.push_back(tenp);



	//for (int i = 0; i < m_particle.size(); ++i) {
	for (auto it = m_particle.begin(); it !=  m_particle.end();) {
		// ベロシティーの更新
		(*it).m_vel.y -= 0.9;
		DirectX::XMFLOAT3 pos = (*it).game->GetPosition();

		pos.x += (*it).m_vel.x;
		pos.y -= (*it).m_vel.y;

		(*it).game->SetPosition(pos);
		(*it).game->GetComponent<Sprite>()->SetPos((int)pos.x,(int)pos.y);


		if ((*it).game->GetPosition().y > SCREEN_HEIGHT - 10.0f){
			(*it).game->Destroy();
			it = m_particle.erase(it);
			continue;
		}
		++it;
	}







	if (m_Transition->GetState() == Transition::State::Stop) {
		if (Input::GetKeyTrigger(VK_RETURN) || Input::GetKeyTrigger(VK_SPACE)) {
			//Manager::SetScene<Game>();
			m_Transition->FadeOut();

		}
	}
	if (m_Transition->GetState() == Transition::State::Finish) {
		Manager::SetScene<Game>();
	}
}
