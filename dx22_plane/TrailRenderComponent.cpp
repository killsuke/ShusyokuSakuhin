#include "TrailRenderComponent.h"
#include "Transform.h"
#include "DirectXRender.h"
#include "Mesh/TrailMesh.h"
#include <vector>
#include <iostream>

using namespace DirectX::SimpleMath;
using namespace DirectX;

namespace {
	constexpr float DeltaTime = 0.016f;
}

TrailRenderComponent::TrailRenderComponent(GameObject& obj) : RenderComponent(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("RENDER"); // ソート番号を設定
	m_Shader = std::make_unique<Shader>();
	CreateMesh<TrailMesh>();
	SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
}

void TrailRenderComponent::Update() {

	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();
	ArbitraryRotationComponent* arbitraryRotation = m_Object->GetComponent<ArbitraryRotationComponent>();
	const float rollingSpeed = arbitraryRotation->GetRollingSpeed();

	int pointSize = static_cast<int>(m_TrailPoints.size());

	if (transform != nullptr) {

		if (pointSize < 2) return;

		InversionEvent();

		std::vector<VERTEX_3D> vertices;
		std::vector<unsigned int> indices;

		vertices.reserve(pointSize * 2);
		indices.reserve((pointSize - 1) * 6);

		for (size_t i = 0; i + 1 < m_TrailCount; ++i) {

			const TrailPoint p0 = m_TrailPoints[i];
			const TrailPoint p1 = m_TrailPoints[i + 1];

			const Vector4 color0 = Color(1.0f, 1.0f, 1.0f, 1.0f - p0.lifeTime); // ポイントの生存時間に基づいて透明度を設定
			const Vector4 color1 = Color(1.0f, 1.0f, 1.0f, 1.0f - p1.lifeTime); // ポイントの生存時間に基づいて透明度を設定

			const DirectX::SimpleMath::Vector3 normal(0.0f, 1.0f, 0.0f);

			// UVは縦方向に時間または距離で割り当てるとする
			const float v0 = static_cast<float>(i) / static_cast<float>(pointSize);
			const float v1 = static_cast<float>(i + 1) / static_cast<float>(pointSize);

			vertices.push_back({ p0.basePosition,normal,color0 ,{0.0f,v0} });
			vertices.push_back({ p0.tipPosition,normal,color0 ,{1.0f,v0} });
			vertices.push_back({ p1.basePosition,normal,color1 ,{0.0f,v1} });
			vertices.push_back({ p1.tipPosition,normal,color1 ,{1.0f,v1} });

			const unsigned int start = static_cast<unsigned int>(i) * 4;

			if (m_RightLeftFlag == false) {

				// 表
				indices.push_back(start + 1);
				indices.push_back(start + 0);
				indices.push_back(start + 2);
				indices.push_back(start + 1);
				indices.push_back(start + 2);
				indices.push_back(start + 3);
			}
			else {

				// 反転（表裏逆に）
				indices.push_back(start + 0);
				indices.push_back(start + 1);
				indices.push_back(start + 2);
				indices.push_back(start + 2);
				indices.push_back(start + 1);
				indices.push_back(start + 3);
			}
		}

		// 止めたいときとか
		if (rollingSpeed != 0.0f) {

			// 範囲超えないように
			for (int i = 0; i < m_AverageSamplingNum; ++i) {

				const float color = (1.0f / pointSize) * 1.5f;
				for (size_t i = 0; i < m_TrailCount; ++i) {

					m_TrailPoints[i].lifeTime += color;
				}

				m_TrailCount++;
			}
			if (m_TrailCount > pointSize - 1) {
				m_TrailCount = pointSize - 1;
			}

			// lifeTime 更新の後に実行
			while (!m_TrailPoints.empty() && m_TrailPoints.front().lifeTime >= 1.0f) {
				m_TrailPoints.erase(m_TrailPoints.begin());
				// 削除したのでカウントも調整
				if (m_TrailCount > 0) {
					m_TrailCount--;
				}
			}
		}

		ID3D11DeviceContext* deviceContext = DirectXRender::GetDeviceContext();

		D3D11_MAPPED_SUBRESOURCE mappedVB;
		HRESULT hr = deviceContext->Map(m_VertexBuffer.GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedVB);
		if (SUCCEEDED(hr)) {
			memcpy(mappedVB.pData, vertices.data(), sizeof(VERTEX_3D) * vertices.size());
			deviceContext->Unmap(m_VertexBuffer.GetBuffer(), 0);
		}

		D3D11_MAPPED_SUBRESOURCE mappedIB;
		hr = deviceContext->Map(m_IndexBuffer.GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedIB);
		if (SUCCEEDED(hr)) {
			memcpy(mappedIB.pData, indices.data(), sizeof(unsigned int) * indices.size());
			deviceContext->Unmap(m_IndexBuffer.GetBuffer(), 0);
		}

		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 頂点の結び方の規則
		m_Shader->SetGPU();
		m_VertexBuffer.SetGPU();
		m_IndexBuffer.SetGPU();

		auto texture = m_Mesh->GetTextures();

		auto uvs = texture[0].GetUVSets();

		uvs.x = uvs.x - 1;
		uvs.y = uvs.y - 1;
		uvs.z = 1 / uvs.z;
		uvs.w = 1 / uvs.w;

		//定数バッファを更新
		ConstBuffer cb;

		ArbitraryRotationComponent* arbitrary = m_Object->GetComponent<ArbitraryRotationComponent>();
		if (arbitrary != nullptr) {
			GameObject* centerObj = arbitrary->GetCenterObject();

			const TransformComponent* centerObjTrans = centerObj->GetComponent<TransformComponent>();
			const XMFLOAT3 centerPos = centerObjTrans->GetPosition();

			const XMVECTOR posVec = XMVectorSet(centerPos.x, centerPos.y, centerPos.z, 1.0f);

			const XMMATRIX posMtx = XMMatrixTranslationFromVector(posVec);

			cb.matrixWorld = XMMatrixTranspose(posMtx);
		}
		else {
			cb.matrixWorld = XMMatrixIdentity();
		}

		cb.color = m_Color;

		cb.matrixTex = texture[0].MakeUV(uvs.x, uvs.y, uvs.z, uvs.w);

		cb.inverse = m_InversionFlag;

		// 行列をシェーダーに渡す
		deviceContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);

		texture[0].SetGPU();

		DirectXRender::SetRasterizerState(ERasterizerState::RS_NONE);

		deviceContext->DrawIndexed((unsigned int)indices.size(), 0, 0);
		DirectXRender::SetRasterizerState(ERasterizerState::RS_CULL_BACK);
	}
}

void TrailRenderComponent::TrailUpdate(const DirectX::SimpleMath::Vector3& base, const DirectX::SimpleMath::Vector3& tip) {

	ArbitraryRotationComponent* arbitraryRotation = m_Object->GetComponent<ArbitraryRotationComponent>();
	if (arbitraryRotation != nullptr) {
		const float speed = arbitraryRotation->GetRollingSpeed();

		// スピードが１より下ならそもそも処理しない
		if (speed >= 1.0f) {
			if (!m_TrailPoints.empty()) {

				const TrailPoint prev = m_TrailPoints.back();
				const float dist = (base - prev.basePosition).Length();

				// 一定距離より長い場合は補間
				const float step = 1.0f / speed;	// スピードに応じて変化
				if (dist > step) {
					const int div = static_cast<int>(dist / step);	// サンプリング数を減らして無駄を削る
					for (int i = 1; i < div; ++i) {
						const float time = (float)i / div;
						TrailPoint mid;
						mid.basePosition = Vector3::Lerp(prev.basePosition, base, time);
						mid.tipPosition = Vector3::Lerp(prev.tipPosition, tip, time);
						mid.lifeTime = 0.0f;
						m_TrailPoints.push_back(mid);
					}

					m_SampleDivisions.push_back(div);
				}
			}
		}
	}

	TrailPoint newPoint;
	newPoint.basePosition = base;
	newPoint.tipPosition = tip;
	newPoint.lifeTime = 0.0f; // 初期の生存時間を設定

	m_TrailPoints.push_back(newPoint);
}

void TrailRenderComponent::SetTrailPoint(const std::vector<PosAndQuaternion>& points) {

	m_SampleDivisions.clear();

	m_TrailCount = 0;
	m_TrailPoints.clear();

	XMFLOAT3 base;
	XMFLOAT3 tip;

	for (const PosAndQuaternion& point : points) {

		// ローカル座標取り出し
		const XMFLOAT3 localPos = point.position;
		const XMVECTOR quat = point.quaternion;

		// クォータニオンを回転行列に変換
		const XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(quat);

		// 回転行列の1列目がRightベクトル（X軸方向）
		const XMVECTOR right = rotationMatrix.r[0]; // r[0] は X軸ベクトル

		base = localPos + right * m_BaseOffset;
		tip = localPos + right * m_TipOffset;

		// 頂点となる候補を追加
		TrailUpdate(base, tip);
	}

	ID3D11Buffer* vertexBuffer = m_VertexBuffer.GetBuffer();
	ID3D11Buffer* indexBuffer = m_IndexBuffer.GetBuffer();

	vertexBuffer->Release();
	indexBuffer->Release();

	int pointSize = static_cast<int>(m_TrailPoints.size());

	TrailMesh* trailMesh = dynamic_cast<TrailMesh*>(m_Mesh.get());
	if (trailMesh != nullptr) {

		std::vector<VERTEX_3D> vertexBuffer = trailMesh->CreateMeshVertices(pointSize * 4);
		std::vector<unsigned int> indexBuffer = trailMesh->CreateMeshIndices(pointSize * 6);

		m_VertexBuffer.Create(vertexBuffer);
		m_IndexBuffer.Create(indexBuffer);
	}

	int vecSize = (int)m_SampleDivisions.size();
	// ０で除算するのを防ぐ
	if (vecSize > 0) {
		int nums = 0;
		for (const auto& vec : m_SampleDivisions) {
			nums += vec;
		}

		m_AverageSamplingNum = nums / vecSize;
	}
	else {
		m_AverageSamplingNum = 1;
	}
}

void TrailRenderComponent::InversionEvent() {

	if (m_InversionFlag == true) {

		for (TrailPoint& trail : m_TrailPoints) {
			trail.basePosition.x *= -1.0f;
			trail.tipPosition.x *= -1.0f;
		}

		m_InversionFlag = false;
	}
}