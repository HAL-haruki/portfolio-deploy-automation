#pragma once
#include <DirectXMath.h>
#include <iostream>
#include "gameObject.h"
#include "player.h"
#include "enemy.h"

class UDP
{
public:
	// シングルトン
	UDP(const UDP&) = delete;
	UDP& operator = (const UDP&) = delete;
	UDP(UDP&&) = delete;
	UDP& operator=(UDP&&) = delete;

	static UDP& get_instance() {
		create();
		return *instance;
	}

	static void create() {
		if (!instance) {
			instance = new UDP;
		}
	}

	static void destroy() {
		delete instance;
		instance = nullptr;
	}

	// クラスの処理
	void InitUdp();
	void UninitUdp();
	
	void SetPlayer(DirectX::XMFLOAT3 playerPos, DirectX::XMFLOAT3 playerRate, bool Attack, bool Hit, int HP);
	DirectX::XMFLOAT3 GetPlayerPos();
	DirectX::XMFLOAT3 GetPlayerRate();
	DirectX::XMFLOAT3 GetPlayerPos2();
	DirectX::XMFLOAT3 GetPlayerRate2();
	bool GetAttack();
	bool GetHit();
	int GetHP();
	bool GetAttackEnemy();
	bool GetHitEnemy();
	int GetHPEnemy();
	void SetEnemy(DirectX::XMFLOAT3 enemyPos, DirectX::XMFLOAT3 enemyRate, bool Attack, bool Hit, int HP);
	void sendMatchRequest();
	void sendPlayerState();
	bool SetMach();
	
	~UDP();
private:
	UDP();

	static UDP* instance;
	DirectX::XMFLOAT3 m_PlayerPos;
	DirectX::XMFLOAT3 m_PlayerRate;
	DirectX::XMFLOAT3 m_EnemyPos;
	DirectX::XMFLOAT3 m_EnemyRate;
	bool m_Attack;
	bool m_Hit;
	int m_HP;
	bool m_AttackEnemy;
	bool m_HitEnemy;
	int m_HPEnemy;

};
void RecvClientData(void* p1);// クライアントデータ受信