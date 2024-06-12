#pragma once

#include "gameObject.h"

class Player : public GameObject
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
	int m_HP;
	bool m_alive = true;
	int m_Frame = 0;
	//int m_push = 0;
	int m_AtacckFrame = 0;
	int m_currentFrame = 0;
	int m_Ataccktime = 0;
	bool m_Attack = false;
	bool m_Hit = false;
	enum class Animetion
	{
		Idol,
		Run,
		Atacck
	} m_animetion;
};
