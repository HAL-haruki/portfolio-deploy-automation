
#include "manager.h"
#include "renderer.h"

#include "input.h"

#include "camera.h"
#include "sky.h"
#include "field.h"
#include "meshField.h"
#include "player.h"
#include "enemy.h"
#include "weapon.h"
#include "explosion.h"
#include "box.h"
#include "cylinder.h"
#include "result.h"
#include "resultclear.h"
#include "title.h"
#include "transition.h"
#include "score.h"
#include "score2.h"
#include "audio.h"
#include "UDP.h"

#include "game.h"



void Game::Init()
{

	Explosion::Load();

	AddGameObject<Camera>(0);
	AddGameObject<Camera>(0);
	AddGameObject<Sky>(1);
	AddGameObject<MeshField>();

	{
		Box* box = AddGameObject<Box>();
		box->SetPosition(DirectX::XMFLOAT3(-50.0f, -1.0f, 0.0f));
		box->SetScale(DirectX::XMFLOAT3(2.0f, 5.0f, 50.0f));
	}
	{
		Box* box = AddGameObject<Box>();
		box->SetPosition(DirectX::XMFLOAT3(50.0f, -1.0f, 0.0f));
		box->SetScale(DirectX::XMFLOAT3(2.0f, 5.0f, 50.0f));
	}
	{
		Box* box = AddGameObject<Box>();
		box->SetPosition(DirectX::XMFLOAT3(0.0f, -1.0f, 50.0f));
		box->SetScale(DirectX::XMFLOAT3(50.0f, 5.0f, 2.0f));
	}
	{
		Box* box = AddGameObject<Box>();
		box->SetPosition(DirectX::XMFLOAT3(0.0f, -1.0f, -50.0f));
		box->SetScale(DirectX::XMFLOAT3(50.0f, 5.0f, 2.0f));
	}
	// プレイヤー
	AddGameObject<Player>();
	AddGameObject<Weapon>();
	// プレイヤー②
	AddGameObject<Enemy>()->SetPosition(DirectX::XMFLOAT3(-5, 0, 4));
//	AddGameObject<Player>()->SetPosition(DirectX::XMFLOAT3(-5, 0, 4));;
//	AddGameObject<Weapon>()->SetPosition(DirectX::XMFLOAT3(-5, 0, 4));;


	AddGameObject<Score>(4);
	AddGameObject<Score2>(4);

	m_BGM = AddGameObject<GameObject>(1)->AddComponent<Audio>();
	m_BGM->Load("asset/audio/maou_game_medley01.wav");
	m_BGM->Play(true);

	m_Transition = AddGameObject<Transition>(4);
	m_Transition->FadeIn();

	m_screen = false;

	UDP::get_instance().SetPlayer({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f ,0.0f }, false, false, 5);
	UDP::get_instance().SetEnemy({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f ,0.0f }, false, false, 5);

	m_isOnce = false;
	m_isOnce2 = false;
}

void Game::Uninit()
{
	Explosion::Unload();
}

void Game::Update()
{
	// プレイヤーのHPが0になった時シーン遷移
	Player* player = GetGameObject<Player>();
	bool alive = player->alive();

	if (alive == false && m_isOnce == false) {
		m_isOnce = true;
		Invoke([=]() { m_Transition->FadeOut(); }, 500);	// フェードインまでの時間
	}

	if (m_Transition->GetState() == Transition::State::Finish && alive == false) {
		Manager::SetScene<Result>();
		m_screen = true;
		return;
	}

	Enemy* enemy = GetGameObject<Enemy>();
	bool alive2 = enemy->alive();

	 
	if (alive2 == false && m_isOnce2 == false) {
		m_isOnce2 = true;
		Invoke([=]() { m_Transition->FadeOut(); }, 500);	// フェードインまでの時間
	}

	if (m_Transition->GetState() == Transition::State::Finish && alive2 == false) {
		Manager::SetScene<ResultClear>();
		m_screen = true;
		return;
	}

	// デバッグ用
#ifdef DEBUG
	if (m_Transition->GetState() == Transition::State::Stop) {
		if (Input::GetKeyTrigger(VK_RETURN)) {
			m_Transition->FadeOut();
		}
}
}
#endif // DEBUG



	if (m_Transition->GetState() == Transition::State::Finish && m_Clear == true) {
		Manager::SetScene<ResultClear>();
	}
}
