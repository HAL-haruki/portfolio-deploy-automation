// [manager.cpp]
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
//#include "game.h"
#include "audio.h"
#include "title.h"

Scene* Manager::m_Scene{};

void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	Audio::InitMaster();

	//m_Scene = new Game();
	//m_Scene->InitBase();
	SetScene<Title>();
}


void Manager::Uninit()
{
	m_Scene->UninitBase();
	delete m_Scene;

	Audio::UninitMaster();
	Input::Uninit();
	Renderer::Uninit();
}

void Manager::Update()
{
	Input::Update();

	m_Scene->UpdateBase();
}

void Manager::Draw()
{
	Renderer::Begin();

	m_Scene->DrawBase();

	Renderer::End();
}
