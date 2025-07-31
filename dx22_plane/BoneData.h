#pragma once
#include <SimpleMath.h>
#define BONE_NUM (15)		// ボーンの数（最大14個まで）
#define INCH_WORM_BONE_NUM (2)

// 最低限の頂点情報
//  座標と各ボーンの重みとしてボーン行列番号があればスキンメッシュはできます！
struct AnimationVertex {
	DirectX::SimpleMath::Color color = {};

	unsigned int matrixIndex[4] = {};

	DirectX::SimpleMath::Vector3 position = {};
	DirectX::SimpleMath::Vector3 weight = {};
	float pad1 = 0.0f;

	float pad2 = 0.0f;
};

struct LineVertex {
	DirectX::SimpleMath::Vector3 pos = {};					// 頂点座標
	DirectX::XMFLOAT4 color = {};					// 色
};

// 定数バッファ（ボーン行列用）
struct CBBoneMatrix {
	// 頂点カラー行列
	//DirectX::XMFLOAT4 color = {1.0f,1.0f,1.0f,1.0f};
	//// UV座標移動行列
	//DirectX::XMMATRIX matrixTex = DirectX::XMMatrixIdentity();

	// ワールド変換行列
	DirectX::SimpleMath::Matrix matrixWorld = DirectX::XMMatrixIdentity();

	// ボーン行列配列
	DirectX::SimpleMath::Matrix		mtx[INCH_WORM_BONE_NUM] = {};
};

// ボーン構造体
struct Bone {
	int id = 0;						// ボーンID（通し番号）
	Bone* firstChild = nullptr;       // 第1子ボーン
	Bone* sibling = nullptr;          // 次の兄弟ボーン
	DirectX::SimpleMath::Matrix offsetMtx;				// ボーンオフセット行列
	DirectX::SimpleMath::Matrix initMtx;					// 初期姿勢行列（ジョイントをどの配置し、どれだけ回転させておくかを表す行列）
	DirectX::SimpleMath::Matrix  boneMtx;				// ボーン姿勢行列
	DirectX::SimpleMath::Matrix* combMtxAry = nullptr;		// 合成姿勢行列配列へのポインタ

	Bone() {
		initMtx = DirectX::SimpleMath::Matrix::Identity;	// 初期姿勢行列を単位行列に設定
		offsetMtx = DirectX::SimpleMath::Matrix::Identity;	// ボーンオフセット行列を単位行列に設定
		boneMtx = DirectX::SimpleMath::Matrix::Identity;	// ボーン姿勢行列を単位行列に設定
	}
};