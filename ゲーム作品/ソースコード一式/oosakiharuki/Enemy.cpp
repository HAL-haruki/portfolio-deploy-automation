#include "manager.h"
#include "input.h"


#include "shader.h"
//#include "modelRenderer.h"
#include "animationModel.h"
#include "shadow.h"
#include "audio.h"
#include "enemy.h"
#include "box.h"
#include "score2.h"
#include "weapon.h"
#include "cylinder.h"
#include "explosion.h"
#include "meshField.h"

#include "UDP.h"

void Enemy::Init()
{
	AddComponent<Shader>()->Load("shader/vertexLightingVS.cso", "shader/vertexLightingPS.cso");
	m_Model = AddComponent<AnimationModel>();
	m_Model->Load("asset/model/Maria WProp J J Ong.fbx");
	m_Model->LoadAnimation("asset/model/sword and shield Run.fbx", "Run");//1�ڂ̃A�j���[�V����
	m_Model->LoadAnimation("asset/model/Sword And Shield Idle.fbx", "Idel");//2�ڂ̃A�j���[�V����
	m_Model->LoadAnimation("asset/model/Standing Melee Attack Downward (3).fbx", "Atacck");//3�ڂ̃A�j���[�V����

	AddComponent<Shadow>()->SetSize(1.5f);


	m_SE = AddComponent<Audio>();
	m_SE->Load("asset/audio/se_sword11.wav");

	SetPosition(DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f));
	SetScale(DirectX::XMFLOAT3(0.02f, 0.02f, 0.02f));

	m_HP = 5;
}

void Enemy::Update()
{
	m_animetion = Animetion::Idol;

	DirectX::XMFLOAT3 oldPosition = m_Position;
	m_ReceivedPos =  UDP::get_instance().GetPlayerPos2();
	m_ReceivedRote = UDP::get_instance().GetPlayerRate2();
	m_Attack = UDP::get_instance().GetAttackEnemy();
	//m_Rotation = m_ReceivedRote;

	if (m_ReceivedPos.x != m_Position.x) {
		m_Position.x = m_ReceivedPos.x;
		m_animetion = Animetion::Run;

	}
	else if (m_ReceivedPos.y != m_Position.y) {
		m_Position.y = m_ReceivedPos.y;
		m_animetion = Animetion::Run;

	}
	else if (m_ReceivedPos.z != m_Position.z) {
		m_Position.z = m_ReceivedPos.z;
		m_animetion = Animetion::Run;

	}
	else if (m_ReceivedRote.x != m_Rotation.x) {
		m_Rotation.x = m_ReceivedRote.x;
		m_animetion = Animetion::Run;

	}
	else if (m_ReceivedRote.y != m_Rotation.y) {
		m_Rotation.y = m_ReceivedRote.y;
		m_animetion = Animetion::Run;

	}
	else if (m_ReceivedRote.z != m_Rotation.z) {
		m_Rotation.z = m_ReceivedRote.z;
		m_animetion = Animetion::Run;

	}
	else {
		m_animetion = Animetion::Idol;
	}
	
	if (m_isOnce == false) {
		m_attackFrame = 0;
	}
	if (m_Attack == true) {
		m_isOnce = m_Attack;
		m_animetion = Animetion::Atacck;
	}
	else if (m_Attack == false) {
		m_isOnce = false;
	}

	
	m_Frame++;
	m_attackFrame++;
	
}


void Enemy::PreDraw()
{
	if (m_animetion == Animetion::Idol) {
		m_Model->Update("Idel", m_Frame / 3);
	}
	else if (m_animetion == Animetion::Run) {
		m_Model->Update("Run", m_Frame / 3);
	}
	else if (m_animetion == Animetion::Atacck) {
		if (m_attackFrame <= 40) {
			m_Model->Update("Atacck", m_attackFrame);
		}
		else {
			m_animetion = Animetion::Idol;
			m_Model->Update("Idel", m_Frame / 3);
		}
	}
}

DirectX::XMFLOAT3 Enemy::GetPlayerPos()
{
	return m_PlayerPos;
}

bool Enemy::alive() {
	return m_alive;
}

void Enemy::TakeDamage(int damageAmount)
{
	m_HP -= damageAmount;

	// �v���C���[���_���[�W���󂯂��ۂ̏�����ǉ�
	if (m_HP <= 0) {
		// �v���C���[��HP��0�ȉ��ɂȂ����ꍇ�̏����i��: �Q�[���I�[�o�[�j
		m_alive = false;
	}

	Scene* scene = Manager::GetScene();
	Score2* score2 = scene->GetGameObject<Score2>();
	score2->SetCount(m_HP);
}