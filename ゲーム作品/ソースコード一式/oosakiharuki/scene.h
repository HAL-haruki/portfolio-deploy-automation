#pragma once

#include <array>
#include <list>
#include <vector>
#include <typeinfo>
#include "gameObject.h"
#include "modelRenderer.h"

class Scene
{
protected:
	//std::list<GameObject*>	m_GameObject;
	std::array<std::list<GameObject*>, 5> m_GameObject;

public:
	Scene() {}
	virtual ~Scene() {}


	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}

	void InitBase()
	{
		Init();
	}

	void UninitBase()
	{
		for (auto& objectList : m_GameObject) {
			for (GameObject* object : objectList) {
				object->UninitBase();
				delete object;
			}
			objectList.clear();
		}
		Uninit();

		ModelRenderer::UnloadAll();
	}

	void UpdateBase()
	{
		for (auto& objectList : m_GameObject) {
			for (GameObject* object : objectList) {
				object->UpdateBase();
			}
			objectList.remove_if([](GameObject* object) { return object->Destroy(); });
		}
		Update();

	}

	void DrawBase()
	{
		DirectX::XMFLOAT4X4 matrix;
		DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixIdentity());

		for (auto& objectList : m_GameObject) {
			for (GameObject* object : objectList) {
				object->DrawBase(matrix);
			}
		}
		Draw();
	}




	template <typename T> //テンプレート関数
	T* AddGameObject(int Layer = 2) {
		T* gameObject = new T();
		m_GameObject[Layer].push_back(gameObject);
		gameObject->Init();

		return gameObject;
	}

	template <typename T>
	T* GetGameObject()
	{
		for (auto& objectList : m_GameObject) {
			for (GameObject* object : objectList)
			{
				if (typeid(*object) == typeid(T))//型を調べる(RTTI動的型情報)
				{
					return (T*)object;
				}
			}
		}
		return nullptr;
	}

	template <typename T>
	std::vector<T*> GetGameObjects()
	{
		std::vector<T*> objects;//STLの配列
		for (auto& objectList : m_GameObject) {
			for (GameObject* object : objectList)
			{
				if (typeid(*object) == typeid(T))
				{
					objects.push_back((T*)object);
				}
			}
		}
		return objects;
	}
};
