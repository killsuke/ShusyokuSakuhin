#pragma once
#include <SimpleMath.h>
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "ModelManager.h"
#include "sound.h"
#include "Component.h"
#include "RenderElement.h"
#include <memory>
#include <string>

class Component; // 前方宣言

class GameObject {
protected:
	std::vector<std::unique_ptr<Component>> components;

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
//	ModelManager& modelManager = ModelManager::GetInstance();	


public:
	
	GameObject();
	GameObject(const std::string& _name,const std::string& _tag)
		: name(_name), tag(_tag) {
	}; // 名前とタグを指定して初期化
//	virtual ~GameObject();		// デストラクタ
	~GameObject();	// デフォルトデストラクタ

//	virtual void Init() = 0;
//	virtual void Update() = 0;
	void Update();
//	virtual void Draw() = 0;
//	virtual void Uninit() = 0;

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

	// セッター
	inline void SetDeleteFg(const bool deletefg) { this->deletefg = deletefg; };
	inline void SetTag(const std::string& tag) { this->tag = tag; };
	inline void SetName(const std::string& name) { this->name = name; };

	// ゲッター
	inline bool GetDeleteFg()const { return deletefg; };
	inline std::string& GetTag() { return tag; };
	inline std::string& GetName() { return name; };

	//template<typename T1>
	//// コンポーネントを追加する
	//T1* AddComponent() {
	//	static_assert(std::is_base_of<Component, T1>::value, 
	//		"型エラー！Compnentクラスを継承していません！");	// プロジェクトをUTF-8に変換しておく

	//	auto comp = std::make_unique<T1>();
	//	T1* ptr = comp.get();	// 一度別で格納してアクセス違反を防ぐ
	//	components.emplace_back(std::move(comp));
	//	return ptr;
	//}

	// コンポーネントのソート番号でソート
	void SortComponents();	

	// 装備されているコンポーネントを取得して使用可能にする
	template<typename T2>
	T2* GetComponent() {
		for (auto& component : components) { // ゲームオブジェクト内のコンポーネントをループで見る
			if (auto ptr = dynamic_cast<T2*>(component.get())) {	// ダイナミックキャストでキャスト可能かどうか判定
				return ptr;
			}
		}
		return nullptr; // 指定された型がなかった場合nullptr
	}

	template<typename T3>
	// コンポーネントを追加する
	T3* AddComponent(GameObject& obj) {
		static_assert(std::is_base_of<Component, T3>::value,
			"型エラー！Compnentクラスを継承していません！");	// プロジェクトをUTF-8に変換しておく

		auto comp = std::make_unique<T3>(obj);
		T3* ptr = comp.get();	// 一度別で格納してアクセス違反を防ぐ
		components.emplace_back(std::move(comp));
		SortComponents();
		return ptr;
	}
};
