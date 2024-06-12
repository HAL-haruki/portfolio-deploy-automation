#include "manager.h"
#include "input.h"


#include "shader.h"
//#include "modelRenderer.h"
#include "animationModel.h"
#include "shadow.h"
#include "audio.h"

#include "player.h"
#include "box.h"
#include "score.h"
#include "weapon.h"
#include "cylinder.h"
#include "explosion.h"
#include "meshField.h"
#include "UDP.h"
#include "Massage.h"

void Player::Init()
{
	AddComponent<Shader>()->Load("shader/vertexLightingVS.cso", "shader/vertexLightingPS.cso");
	m_Model = AddComponent<AnimationModel>();
	m_Model->Load("asset/model/Maria WProp J J Ong.fbx");
	m_Model->LoadAnimation("asset/model/sword and shield Run.fbx", "Run");//1つ目のアニメーション
	m_Model->LoadAnimation("asset/model/Sword And Shield Idle.fbx", "Idel");//2つ目のアニメーション
	m_Model->LoadAnimation("asset/model/Standing Melee Attack Downward (3).fbx", "Atacck");//3つ目のアニメーション

	AddComponent<Shadow>()->SetSize(1.5f);

	m_SE = AddComponent<Audio>();
	m_SE->Load("asset/audio/se_sword11.wav");

	SetPosition(DirectX::XMFLOAT3(-3.0f, 0.5f, 0.0f));
	SetScale(DirectX::XMFLOAT3(0.02f, 0.02f, 0.02f));

	m_HP = 5;

	UDP::get_instance().sendMatchRequest();

}

void Player::Update()
{
	if (m_currentFrame == 0 or m_currentFrame >= 40) {
		m_animetion = Animetion::Idol;
	}
	DirectX::XMFLOAT3 oldPosition = m_Position;
	if (Input::GetKeyPress('A'))
	{
		m_Rotation.y -= 0.02f;
		m_animetion = Animetion::Run;
	}
	if (Input::GetKeyPress('D'))
	{
		m_Rotation.y += 0.02f;
		m_animetion = Animetion::Run;
	}
	// 前後移動
	m_move = GetForward();	// 前後ベクトル
	if (Input::GetKeyPress('W'))
	{
		m_Position.x += m_move.x * 0.1f;
		m_Position.y += m_move.y * 0.1f;
		m_Position.z += m_move.z * 0.1f;
		m_animetion = Animetion::Run;
	}
	if (Input::GetKeyPress('S'))
	{
		m_Position.x -= m_move.x * 0.1f;
		m_Position.y -= m_move.y * 0.1f;
		m_Position.z -= m_move.z * 0.1f;
		m_animetion = Animetion::Run;
	}


	//重力
	m_Velocity.y -= 0.015f;

	//抵抗
	m_Velocity.y -= m_Velocity.y * 0.01f;

	//移動
	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
	m_Position.z += m_Velocity.z;

	// メッシュフィールドの高さを取得
	MeshField* meshField = Manager::GetScene()->GetGameObject<MeshField>();
	m_Position.y = meshField->GetHeight(m_Position);

	//接地
	float groundHeight = 0.0f;
	
	Scene* scene = Manager::GetScene();
	{

		std::vector<Cylinder*> cylinderList = scene->GetGameObjects<Cylinder>();
		for (Cylinder* cylinder : cylinderList)
		{
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
				if (m_Position.y < position.y + scale.y - 0.5f)
				{
					m_Position.x = oldPosition.x;
					m_Position.z = oldPosition.z;
				}
				else
				{
					groundHeight = position.y + scale.y;
				}
				break;
			}
		}
	}
	{
		std::vector<Box*> boxList = scene->GetGameObjects<Box>();
		for (Box* box : boxList)
		{
			DirectX::XMFLOAT3 position = box->GetPosition();
			DirectX::XMFLOAT3 scale = box->GetScale();
			if (position.x - scale.x - 0.5f < m_Position.x &&
				m_Position.x < position.x + scale.x + 0.5f &&
				position.z - scale.z - 0.5f < m_Position.z &&
				m_Position.z < position.z + scale.z + 0.5f)
			{
				if (m_Position.y < position.y + scale.y * 2.0f - 0.5f)
				{
					m_Position.x = oldPosition.x;
					m_Position.z = oldPosition.z;
				}
				else
				{
					groundHeight = position.y + scale.y * 2.0f;
				}
			}
		}
	}

	if (m_Position.y < groundHeight && m_Velocity.y < 0.0f)
	{
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;
		if (Input::GetKeyTrigger(VK_SPACE))
		{
			m_Velocity.y = 0.35f;
		}
	}

		Weapon* weapon = scene->GetGameObject<Weapon>();
	if (Input::GetKeyTrigger('K') && m_Ataccktime > 60) {
		m_Ataccktime = 0;
		m_AtacckFrame = 0;
		m_animetion = Animetion::Atacck;
		weapon->Attack();
		m_currentFrame = 0;
		m_Attack = true;
	}
		m_Hit = weapon->Hit();

	// 送信処理
// 受信前
	if (UDP::get_instance().SetMach() == false && Input::GetKeyPress(VK_F1)) {
		UDP::get_instance().sendMatchRequest();
	} // 受信後
	else if (UDP::get_instance().SetMach() == true) {
		UDP::get_instance().SetPlayer(m_Position, m_Rotation, m_Attack, m_Hit, m_HP);
		UDP::get_instance().sendPlayerState();
	}


	// プレイヤーの座標更新敵との当たり判定用
	m_PlayerPos = m_Position;

	m_Frame++;
	m_AtacckFrame++;
	m_Ataccktime++;

	m_currentFrame;
	if (m_Ataccktime > 60) {
		m_Attack = false;
	}

	m_Hit = false;

}


void Player::PreDraw()
{
	if (m_animetion == Animetion::Idol) {
		m_Model->Update("Idel", m_Frame / 3);
	}
	else if (m_animetion == Animetion::Run) {
		m_Model->Update("Run", m_Frame / 3);
	}
	else if (m_animetion == Animetion::Atacck) {
		m_currentFrame++;
		m_Model->Update("Atacck", m_AtacckFrame);
	}
}

DirectX::XMFLOAT3 Player::GetPlayerPos()
{
	return m_PlayerPos;
}

bool Player::alive(){
	return m_alive;
}

void Player::TakeDamage(int damageAmount)
{
	m_HP -= damageAmount;

	// プレイヤーがダメージを受けた際の処理を追加
	if (m_HP <= 0) {
		// プレイヤーのHPが0以下になった場合の処理（例: ゲームオーバー）
		m_alive = false;
	}

	Scene* scene = Manager::GetScene();
	Score* score = scene->GetGameObject<Score>();
	score->SetCount(m_HP);
}