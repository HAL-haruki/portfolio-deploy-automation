#pragma once

#include "scene.h"
#include <DirectXMath.h>
#include <vector>
//
//#define SCREEN_WIDTH	1280									// �X�N���[�����r�b�g�}�b�v��
//#define SCREEN_HEIGHT	920									// �X�N���[�����r�b�g�}�b�v����
#define ALPHA_VALUE		128									// �A���t�@�l(8bit)
#define TILE_MAX		10		
#define PARTICLE_MAX		100	

#define OFFSET_X			SCREEN_WIDTH/2		// �X�N���[���̕��I�t�Z�b�g
#define OFFSET_Y			SCREEN_HEIGHT*3/4	// �X�N���[���̍����I�t�Z�b�g




class Title : public Scene
{
private:
	class Transition* m_Transition{};

	//std::vector<DirectX::XMFLOAT2> m_vel;
	typedef struct {
		DirectX::XMFLOAT2 m_vel;
		GameObject* game;
	}Particle;
	std::vector<Particle> m_particle;

	int m_f;

public:
	void Init() override;
	void Update() override;

	
};

