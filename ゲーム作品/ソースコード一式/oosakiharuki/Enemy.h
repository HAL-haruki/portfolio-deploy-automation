#pragma once

#include "gameObject.h"

class Enemy : public GameObject
{
public:
	void Init() override;
	void Update() override;
	void PreDraw() override;
	DirectX::XMFLOAT3 GetPlayerPos();
	bool alive();
	void TakeDamage(int damageAmount);
private:
	DirectX::XMFLOAT3 m_Velocity{};
	class Audio*        m_SE{};
	class AnimationModel* m_Model;

	DirectX::XMFLOAT3 m_move;
	DirectX::XMFLOAT3 m_forward;
	DirectX::XMFLOAT3 m_PlayerPos;

	int m_Frame = 0;
	int m_attackFrame = 0;
	int m_HP;
	bool m_alive = true;
	bool m_Attack;
	bool m_isOnce;

	DirectX::XMFLOAT3 m_ReceivedPos;
	DirectX::XMFLOAT3 m_ReceivedRote;


	enum class Animetion
	{
		Idol,
		Run,
		Atacck
	} m_animetion;
};
