#pragma once
#include "gameObject.h"


class Camera : public GameObject
{
	enum CAMERAANGLE {
		FISTPERSON,	// 一人称
		THIRDPERSON,	// 三人称
		FIXED,		// 固定カメラ
		MAXPERSON
	};

private:

	DirectX::XMFLOAT3	m_Target{};
	DirectX::XMFLOAT4X4	m_ViewMatrix{};

	CAMERAANGLE m_CameraAngle;

public:
	void Init() override;
	void Update() override;
	void Draw() override;

	DirectX::XMFLOAT4X4 GetViewMatrix() { return m_ViewMatrix; }


};
