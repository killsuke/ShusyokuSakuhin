#pragma once
#include "Object.h"
#include <array>

// 板ポリを切断するなら、切断線が表側か裏側か、板ポリと完全に重なっているかを判定
// 完全に重なっているなら切断
struct Line2D {
	float a, b, c;

	// 点（ x , y ）が正側 or 負側かチェック
	float evaluate(float x, float y) const {
		return a * x + b * y + c;
	}
};

struct SlashVertex {
	DirectX::XMFLOAT3 pos;	// 頂点の位置（x,y,z）
	DirectX::XMFLOAT2 uv;	// テクスチャ座標（u,v）
};

class TestBoard :public Object
{
private:
	std::vector<SlashVertex> triangles;	// 切断用の三角形頂点データ
public:
	TestBoard(Camera* cam);
	~TestBoard();

	void Init();
	void Draw();
	void Update();
	void Uninit();

	Line2D MakeLineFromPoints(const DirectX::XMFLOAT2& p1, const DirectX::XMFLOAT2& p2) {
		float dx = p2.x - p1.x;
		float dy = p2.y - p1.y;
		float a = dy;
		float b = -dx;
		float c = -(a * p1.x + b * p1.y);

		return { a,b,c };
	}

	// v1 と v2 の位置を補間する
	// 切断した際に新たな頂点を追加する
	SlashVertex LerpVertex(const SlashVertex& v1, const SlashVertex& v2, float t) {
		SlashVertex v;
		v.pos.x = (1 - t) * v1.pos.x + t * v2.pos.x;
		v.pos.y = (1 - t) * v1.pos.y + t * v2.pos.y;
		v.pos.z = (1 - t) * v1.pos.z + t * v2.pos.z;
		v.uv.x = (1 - t) * v1.uv.x + t * v2.uv.x;
		v.uv.y = (1 - t) * v1.uv.y + t * v2.uv.y;

		return v;
	}

	void CutTriangleWithLine(const SlashVertex& v0, const SlashVertex& v1, const SlashVertex& v2, 
		const Line2D& line, std::vector<SlashVertex>& outTriangles) {
		std::array<SlashVertex, 3> verts = { v0,v1,v2 };
		
		// dは各頂点が切断線からどの位置にあるか（表 or 裏 or 線上）を示す。
		std::array<float, 3> d = {
			line.evaluate(v0.pos.x, v0.pos.y),
			line.evaluate(v1.pos.x, v1.pos.y),
			line.evaluate(v2.pos.x, v2.pos.y),
		};

		// front：切断線の正側にあるポリゴンの頂点群
		// back：切断線の負側にあるポリゴンの頂点群（現在は使わず）
		std::vector<SlashVertex> front, back;

		// 頂点と辺を走査
		for (int i = 0; i < 3; ++i) {

			// 三角形の各辺 curr → next を順にチェック
			const SlashVertex& curr = verts[i];
			const SlashVertex& next = verts[(i + 1) % 3];
			float d0 = d[i];
			float d1 = d[(i + 1) % 3];

			// curr の評価値が正側 → frontに追加
			if (d0 >= 0)front.push_back(curr);
			else		back.push_back(curr);

			// 交差判定
			if ((d0 >= 0 && d1 < 0) || (d0 < 0 && d1 >= 0)) {
				float t = d0 / (d0 - d1);	// 線形補間係数
				SlashVertex inter = LerpVertex(curr, next, t);
				front.push_back(inter);
				back.push_back(inter);
			}
		}

		// front または back のポリゴンを保存（片方でもOK）
		if (front.size() >= 3) {
			for (size_t i = 1; i + 1 < front.size(); ++i) {
				outTriangles.push_back(front[0]);
				outTriangles.push_back(front[i]);
				outTriangles.push_back(front[i + 1]);
			}
		}
	};

	DirectX::SimpleMath::Vector3 GetPos() { return m_Position; };
};

