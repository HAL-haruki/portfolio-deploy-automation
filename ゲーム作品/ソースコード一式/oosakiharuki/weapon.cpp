#include "weapon.h"
#include "input.h"
#include "modelRenderer.h"
#include "shader.h"
#include "shadow.h"
#include "player.h"
#include "enemy.h"
#include "explosion.h"
#include "scene.h"
#include "manager.h"
#include "score.h"
#include "audio.h"
#include "UDP.h"
#include "Header.h"

void Weapon::Init()
{
	//AddComponent<Shader>()->Load("shader/vertexLightingVS.cso", "shader/vertexLightingPS.cso");
	m_SE = AddComponent<Audio>();
	m_SE->Load("asset/audio/se_sword11.wav");
	//AddComponent<ModelRenderer>()->Load("asset/model/untitled.obj");
	//SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	//m_Scale = { 0.01f, 0.01f, 0.01f };
	isAttacking = false;
	attackCooldown = 0.0f;
	attackCooldownTime = 1.0f;
	m_count = 0;
	m_time = 100;

}

void Weapon::Update()
{
	m_Hitp1 = false;

	if (isAttacking) {

		// 剣の攻撃範囲内にいる敵を処理するコード
		Scene* scene = Manager::GetScene();
		std::vector<Enemy*> EnemyList = scene->GetGameObjects<Enemy>();

		for (Enemy* Enemy : EnemyList) {
			if (Enemy->IsDestroy()) continue;
			DirectX::XMFLOAT3 position = Enemy->GetPosition();
			DirectX::XMFLOAT3 direction;
			direction.x = position.x - m_Position.x;
			direction.y = position.y - m_Position.y;
			direction.z = position.z - m_Position.z;

			float lengthSq;
			DirectX::XMStoreFloat(&lengthSq, DirectX::XMVector3LengthSq(DirectX::XMLoadFloat3(&direction)));
			PrintLog("長さ：lengthSq %6.2f\n",sqrtf(lengthSq));

			// 剣の攻撃範囲内に敵がいるかを判定
			if (lengthSq < (m_AttackRange * m_AttackRange)) {// 剣の攻撃が当たった場合
				Explosion* explosion = scene->AddGameObject<Explosion>(3);
				explosion->SetScale(DirectX::XMFLOAT3(1.5f, 1.5f, 1.0f));
				explosion->SetPosition(DirectX::XMFLOAT3(position.x, position.y + 2.0f, position.z));

				Score* score = scene->GetGameObject<Score>();
				m_SE->Play();
				Enemy->TakeDamage(1);
				m_Hitp1 = true;
				m_count++;
				
			}
		}

		// クールダウン時間を減少させる
		attackCooldown -= 1.0f;

		// クールダウン時間が0以下になったら攻撃終了
		if (attackCooldown <= 0.0f) {
			isAttacking = false;
			attackCooldown = 0.0f; // クールダウン時間をリセット
		}
	}
	else {
		m_time++;
		if (m_time > 30.0f){
		Scene* scene = Manager::GetScene();
		std::vector<Player*> playerList = scene->GetGameObjects<Player>();
		if (!playerList.empty()) {
			Player* player = playerList[0];
			DirectX::XMFLOAT3 forward = player->GetForward();
			DirectX::XMFLOAT3 position = player->GetPosition();
			DirectX::XMFLOAT3 pRotation = player->GetRotation();
			m_Position.x = forward.x * 2 + position.x;
			m_Position.y = forward.y + 2 + position.y;
			m_Position.z = forward.z * 2 + position.z;
			m_Rotation.x = pRotation.x - DirectX::XMConvertToRadians(60.0f);
			m_Rotation.y = pRotation.y;
			m_Rotation.z = pRotation.z;
		}
		}
	}

	m_Hitp2 = UDP::get_instance().GetHitEnemy();
	if (m_Hitp2 == true) {
		PrintLog("当たった！！");
		Scene* scene = Manager::GetScene();
		std::vector<Player*> playerList = scene->GetGameObjects<Player>();

		for (Player* player : playerList) {
			if (player->IsDestroy()) continue;
			DirectX::XMFLOAT3 position = player->GetPosition();

			Explosion* explosion = scene->AddGameObject<Explosion>(3);
			explosion->SetScale(DirectX::XMFLOAT3(1.5f, 1.5f, 1.0f));
			explosion->SetPosition(DirectX::XMFLOAT3(position.x, position.y + 2.0f, position.z));

			Score* score = scene->GetGameObject<Score>();
			m_SE->Play();
			player->TakeDamage(1);
			//m_Hitp1 = true;
			m_count++;

		}
	}
}

void Weapon::Attack() {
	if (!isAttacking && attackCooldown <= 0.0f) {
		isAttacking = true;
		Scene* scene = Manager::GetScene();
		std::vector<Player*> playerList = scene->GetGameObjects<Player>();
		if (!playerList.empty()) {
			Player* player = playerList[0];
			DirectX::XMFLOAT3 forward = player->GetForward();
			DirectX::XMFLOAT3 position = player->GetPosition();
			DirectX::XMFLOAT3 pRotation = player->GetRotation();
			m_Position.x = forward.x * 2 + position.x;
			//m_Position.y = forward.y + 2;
			m_Position.z = forward.z * 2 + position.z;
			m_Rotation = pRotation;
			m_time = 0;
			m_AtacckFrame = 0;
		}
		// クールダウンタイムを設定
		attackCooldown = attackCooldownTime;
	}
}

bool Weapon::Hit()
{
	return m_Hitp1;
}


