#pragma once

#include "gameObject.h"
class Weapon : public GameObject
{

public:

	void Init() override;
	void Update() override;
	void SetAttackRange(float range) { m_AttackRange = range; }
	void Attack();
	bool Hit();

private:
	float m_AttackRange = 3.0f; // Œ•‚ÌUŒ‚”ÍˆÍ
	bool isAttacking;
	float attackCooldown;
	float attackCooldownTime;
	int m_count;
	float m_time;
	int m_AtacckFrame;
	class Audio*        m_SE{};

	bool m_Hitp1;
	bool m_Hitp2;
};


