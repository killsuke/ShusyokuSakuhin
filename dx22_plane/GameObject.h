#pragma once
#include <SimpleMath.h>
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Camera.h"
#include "Collision.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "ModelManager.h"
#include "sound.h"
#include "Component.h"
#include <memory>
#include <string>

class GameObject {
protected:
	std::vector<std::shared_ptr<Component>> components;

	// SRT情報（姿勢情報）
	DirectX::SimpleMath::Vector3 m_Position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3 m_Rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3 m_Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

	// 描画の為の情報（メッシュに関わる情報）
	IndexBuffer	 m_IndexBuffer; // インデックスバッファ
	VertexBuffer<VERTEX_3D>	m_VertexBuffer; // 頂点バッファ

	ID3D11ShaderResourceView* m_pTextureView = nullptr;

	// 描画の為の情報（見た目に関わる部分）
	Texture m_Texture;	// テクスチャ
	Shader m_Shader; // シェーダー

	// カメラ
	Camera* m_Camera = nullptr;

	Collision::AABB coll_ab{};
	Collision::Sphere coll_sp{};

	Collision::Vector9 vec9{};

	std::vector<VERTEX_3D> m_Vertices;	// 頂点情報

	// 描画の為の情報（メッシュに関わる情報）
	MeshRenderer m_MeshRenderer; // 頂点バッファ・インデックスバッファ・インデックス数

	// 描画の為の情報（見た目に関わる部分）
	std::vector<std::unique_ptr<Material>> m_Materiales;
	std::vector<SUBSET> m_subsets;
	std::vector<std::unique_ptr<Texture>> m_Textures; // テクスチャ

	std::string tag = "";	// タグを付けて識別する
	std::string name = "";	// オブジェクトの名前

	bool deletefg = false;	// オブジェクトを削除して良いかどうかのフラグ

	// モデルデータを読み込むマネージャーを保持
	ModelManager& modelManager = ModelManager::GetInstance();	


public:
	
	GameObject() = default;
	GameObject(Camera* cam);	// コンストラクタ
	GameObject(const std::string& _name,const std::string& _tag, Camera* cam = nullptr)
		: m_Camera(cam), name(_name), tag(_tag) {
	}; // 名前とタグを指定して初期化
	virtual ~GameObject();		// デストラクタ

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Uninit() = 0;

	// position セッター関数・ゲッター関数
	inline void SetPosition(const DirectX::SimpleMath::Vector3& pos) { this->m_Position = pos; };
	inline DirectX::SimpleMath::Vector3 GetPosition() const { return m_Position; };
	// size セッター関数・ゲッター関数
	inline void SetScale(const DirectX::SimpleMath::Vector3& size) { this->m_Scale = size; };
	inline DirectX::SimpleMath::Vector3 GetScale() const { return m_Scale; };
	// angle セッター関数・ゲッター関数
	inline void SetRotation(const DirectX::SimpleMath::Vector3& angle) { this->m_Rotation = angle; };
	inline DirectX::SimpleMath::Vector3 GetRotation() const { return m_Rotation; };

	// 頂点情報を取得
	std::vector<VERTEX_3D> GetVertices() { return m_Vertices; };

	Collision::AABB GetColl()const { return this->coll_ab; };

	// セッター
	inline void SetColliderSize_OBB(const DirectX::XMFLOAT3& pos,
		const DirectX::XMFLOAT3& size, const DirectX::XMFLOAT3& angle) {
		// 位置
		this->vec9.x = pos.x;
		this->vec9.y = pos.y;
		this->vec9.z = pos.z;

		// サイズ
		this->vec9.w = size.x;
		this->vec9.h = size.y;
		this->vec9.u = size.z;

		// アングル
		this->vec9.pitch = angle.x;
		this->vec9.yaw = angle.y;
		this->vec9.roll = angle.z;
	};

	// セッター
	inline void SetColliderSize_AABB(const DirectX::XMFLOAT3& pos,
		const DirectX::XMFLOAT3& size) {
		// 最小値
		this->coll_ab.min.x = pos.x - size.x;
		this->coll_ab.min.y = pos.y - size.y;
		this->coll_ab.min.z = pos.z - size.z;

		// 最大値
		this->coll_ab.max.x = pos.x + size.x;
		this->coll_ab.max.y = pos.y + size.y;
		this->coll_ab.max.z = pos.z + size.z;
	};

	inline void SetColliderSize_Sphere(const DirectX::XMFLOAT3& pos,
		const float radius) {
		this->coll_sp.center = pos;
		this->coll_sp.radius = radius;
	};

	// ゲッター
	inline Collision::Vector9& GetColliderSize_OBB() { return this->vec9; };
	inline Collision::AABB& GetColliderSize_AABB() { return this->coll_ab; };
	inline Collision::Sphere& GetColliderSize_Sphere() { return this->coll_sp; };

	// セッター
	inline void SetDeleteFg(const bool deletefg) { this->deletefg = deletefg; };
	inline void SetTag(const std::string& tag) { this->tag = tag; };
	inline void SetName(const std::string& name) { this->name = name; };

	// ゲッター
	inline bool GetDeleteFg()const { return deletefg; };
	inline std::string& GetTag() { return tag; };
	inline std::string& GetName() { return name; };

	// コンポーネントを追加する
	void AddComponent(std::shared_ptr<Component> component) {
		components.push_back(component);	// ここであらゆるコンポーネントを装備することで色々出来る。
	}

	// 装備されているコンポーネントを取得して使用可能にする
	template<typename T>
	std::shared_ptr<T> GetComponent() {
		for (auto& component : components) { // ゲームオブジェクト内のコンポーネントをループで見る
			if (std::shared_ptr<T> t = std::dynamic_pointer_cast<T>(component)) { // ダイナミックキャストでキャスト可能かどうか判定
				return t; // 指定された型のshared_ptrに変換したものを返却
			}
		}
		return nullptr; // 指定された型がなかった場合nullptr
	}

	//template <typename T>
	//void AddComponent(std::shared_ptr<T> component) {
	//	components.emplace_back(component);
	//}

	//template <typename T2>
	//std::shared_ptr<T2> GetComponent() {
	//	for (auto& component : components) { // ゲームオブジェクト内のコンポーネントをループで見る
	//		if (std::shared_ptr<T2> t = std::dynamic_pointer_cast<T>(component)) { // ダイナミックキャストでキャスト可能かどうか判定
	//			return t; // 指定された型のshared_ptrに変換したものを返却
	//		}
	//	}
	//	return nullptr; // 指定された型がなかった場合nullptr
	//}

};
