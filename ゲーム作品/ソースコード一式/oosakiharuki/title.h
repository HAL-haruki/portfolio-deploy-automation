#pragma once

#include "scene.h"
#include <DirectXMath.h>
#include <vector>
//
//#define SCREEN_WIDTH	1280									// スクリーン＆ビットマップ幅
//#define SCREEN_HEIGHT	920									// スクリーン＆ビットマップ高さ
#define ALPHA_VALUE		128									// アルファ値(8bit)
#define TILE_MAX		10		
#define PARTICLE_MAX		100	

#define OFFSET_X			SCREEN_WIDTH/2		// スクリーンの幅オフセット
#define OFFSET_Y			SCREEN_HEIGHT*3/4	// スクリーンの高さオフセット




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

