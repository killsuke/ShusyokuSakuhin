#include "TrailRenderComponent.h"
#include "TransformComponent.h"
#include "System/DirectXRender.h"
#include "Mesh/TrailMesh.h"
#include <vector>
#include <iostream>

using namespace DirectX;

namespace {
	constexpr float DeltaTime = 0.016f;
	constexpr int VertexNumSquare = 4;
	constexpr int IndexNumSquare = 6;
}

TrailRenderComponent::TrailRenderComponent(GameObject& obj) : RenderComponent(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("RENDER"); // ソート番号を設定
	m_Shader = std::make_unique<Shader>();
	CreateMesh<TrailMesh>();
	SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
	m_TrailMakeComponent = m_Object->AddComponent<TrailMakeComponent>();
	m_TrailMakeComponent->SetTrailRenderComponent(this);


}

void TrailRenderComponent::Update() {

	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();

	if(transform == nullptr){
		return;
	}

	const unsigned int pointSize = static_cast<unsigned int>(m_TrailPoints.size());

	if (pointSize == 0) {
		return;
	}

	if (transform != nullptr) {

		TrailUpdate();

		ID3D11DeviceContext* deviceContext = DirectXRender::GetDeviceContext();

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

			const XMVECTOR identityQuat = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
			const XMMATRIX posMtx = XMMatrixTranslationFromVector(posVec);
			const XMMATRIX rotMtx = XMMatrixRotationQuaternion(identityQuat);
			const XMMATRIX scaleMtx = XMMatrixScaling(1.0f, 1.0f, 1.0f);

			cb.matrixWorld = XMMatrixTranspose(scaleMtx * rotMtx * posMtx);
		}
		else {
			cb.matrixWorld = XMMatrixIdentity();
		}

		cb.color = m_Color;

		cb.matrixTex = texture[0].MakeUV(uvs.x, uvs.y, uvs.z, uvs.w);

	//	cb.inverse = m_InversionFlag;

		ID3D11Buffer* buffer = DirectXRender::GetDefaultDrawBuffer();

		// 行列をシェーダーに渡す
		deviceContext->UpdateSubresource(buffer, 0, NULL, &cb, 0, 0);

		texture[0].SetGPU();

		DirectXRender::SetCullingState(ECullingState::CULLING_NONE);

		deviceContext->DrawIndexed(m_TrailIndex, 0, 0);
		DirectXRender::SetCullingState(ECullingState::CULLING_BACK);
	}
}

void TrailRenderComponent::TrailCountUp() {

	const int pointSize = static_cast<int>(m_TrailPoints.size());
	float subtractLife = (1.0f / pointSize);
	ArbitraryRotationComponent* arbitrary = m_Object->GetComponent<ArbitraryRotationComponent>();
	if (arbitrary == nullptr) {
		return;
	}

	const bool isFinish = arbitrary->GetIsFinished();

	// 範囲超えないように
	for (int i = 0; i < m_AverageSamplingNum; ++i) {

		if (m_TrailCount > pointSize - 1) {
			break;
		}
		// ここのlifeTimeの調整は後に
		//float subtractLife = (1.0f / pointSize);
		//ArbitraryRotationComponent* arbitrary = m_Object->GetComponent<ArbitraryRotationComponent>();
		//if (arbitrary != nullptr) {
		//	// 全ての軌跡のパーツが揃っている場合、全てのライフポイントを引いてしまう問題がある
		//	

		//	const bool isFinish = arbitrary->GetIsFinished();

		//	if (isFinish == true) {
		//		subtractLife *= 10.0f;
		//	}
		//	else {
		//		subtractLife *= 1.5f;
		//	}
		//}
		//for (size_t j = 0; j < m_TrailCount; ++j) {

		//	m_TrailPoints[j].lifeTime += subtractLife;
		//}

		// これをもし使いたいなら、到達判定が出たフレームは処理しないとバグる
//		if (isFinish == false) {
		m_TrailCount++;
		//	}
	}


	if (isFinish == true) {
		subtractLife *= 50.0f;
	}
	else {
		subtractLife *= 80.0f;
	}

	for (size_t j = 0; j < m_TrailCount; ++j) {

		m_TrailPoints[j].lifeTime += subtractLife;
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

void TrailRenderComponent::TrailUpdate() {

	ArbitraryRotationComponent* arbitraryRotation = m_Object->GetComponent<ArbitraryRotationComponent>();
	const float rollingSpeed = arbitraryRotation->GetRollingSpeed();

	const size_t pointSize_t = m_TrailPoints.size();
	const unsigned int pointSize = static_cast<unsigned int>(m_TrailPoints.size());

	std::vector<VERTEX_3D> vertices;
	std::vector<unsigned int> indices;

	if (pointSize == 0) {
		return;
	}

	vertices.reserve(pointSize_t * 2);
	indices.reserve((pointSize_t - 1) * 6);

	// ここにTraiCountUpがあった


	for (int i = 0; i + 1 < m_TrailCount; ++i) {

		const TrailPoint p0 = m_TrailPoints[i];
		const TrailPoint p1 = m_TrailPoints[i + 1];

		const XMFLOAT4 color0 = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f - p0.lifeTime); // ポイントの生存時間に基づいて透明度を設定
		const XMFLOAT4 color1 = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f - p1.lifeTime); // ポイントの生存時間に基づいて透明度を設定

		const XMFLOAT3 normal(0.0f, 1.0f, 0.0f);

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

	m_TrailIndex = (unsigned int)indices.size();
}

// トレイルのポイントを追加・補間する関数
// サイン関数を使って、中央が一番大きく、端が小さくなるようにオフセットを調整
//void TrailRenderComponent::AddTrailPoints(const XMFLOAT3& center, const XMVECTOR& quaternion, const float trailSpeed) {
//
//	if (!m_TrailPoints.empty()) {
//		const TrailPoint& prev = m_TrailPoints.back();
//		const XMVECTOR quatPrev = prev.localQuat;
//
//		// SLERPで補間
//		const float step = 0.1f;
//		const XMVECTOR centerPrev = XMLoadFloat3(&prev.centerLocalPosition);
//		const XMVECTOR centerNow = XMLoadFloat3(&center);
//
//		// 前と次のセンターの距離を測って、一定距離ごとに補間点を追加
//		const float length = XMVectorGetX(XMVector3Length(centerNow - centerPrev));
//	//	const int div = std::max(1, (int)(length / step));
//
//		const int div = std::max(1, m_TrailDivisionsCount);
//
//		for (int i = 1; i < div; ++i) {
//			const float t = (float)i / div;
//
//			// SLERPで回転補間
//			const XMVECTOR quatInterp = XMQuaternionSlerp(quatPrev, quaternion, t);
//			const XMMATRIX rotMtx = XMMatrixRotationQuaternion(quatInterp);
//			const XMVECTOR right = rotMtx.r[0];
//
//			float offsetValue = 0.0f;
//			// 速度に応じてオフセット量を調整
//			if (trailSpeed >= 1.0f) {
//				offsetValue = trailSpeed * (trailSpeed * 0.001f);
//			}
//
//			// sinでずらし量を調整（πを掛けると中央が最大になる）
//			float offsetScale = sinf(t * XM_PI) * offsetValue;
//			XMVECTOR newCenter = XMVectorLerp(centerPrev, centerNow, t);
//
//			// 一旦見づらいので封印
//			// 向きに合わせてオフセットでズラす
//			newCenter += right * offsetScale;
//
//			const XMVECTOR basePos = newCenter + right * m_BaseOffset;
//			const XMVECTOR tipPos = newCenter + right * m_TipOffset;
//
//			TrailPoint mid;
//			XMStoreFloat3(&mid.centerLocalPosition, newCenter);
//			XMStoreFloat3(&mid.basePosition, basePos);
//			XMStoreFloat3(&mid.tipPosition, tipPos);
//			mid.localQuat = quatInterp;
//			mid.lifeTime = 0.0f;
//			m_TrailPoints.push_back(mid);
//		}
//
//		m_SampleDivisions.push_back(div);
//	}
//
//	// 最後に今フレームの点を push
//	const XMMATRIX rotMtx = XMMatrixRotationQuaternion(quaternion);
//	const XMVECTOR right = rotMtx.r[0];
//	const XMVECTOR centerVec = XMLoadFloat3(&center);
//
//	TrailPoint newPoint;
//	XMStoreFloat3(&newPoint.centerLocalPosition, centerVec);
//	XMStoreFloat3(&newPoint.basePosition, centerVec + right * m_BaseOffset);
//	XMStoreFloat3(&newPoint.tipPosition, centerVec + right * m_TipOffset);
//	newPoint.localQuat = quaternion;
//	newPoint.lifeTime = 0.0f;
//	m_TrailPoints.push_back(newPoint);
//}


void TrailRenderComponent::SetTrailPoint(const std::vector<PosAndQuaternion>& points) {

	m_SampleDivisions.clear();

	m_TrailCount = 0;
	m_TrailPoints.clear();

	m_AverageSamplingNum = 0;
	m_TrailIndex = 0;

	ArbitraryRotationComponent* arbitraryRotation = m_Object->GetComponent<ArbitraryRotationComponent>();
	const float rollingSpeed = arbitraryRotation->GetRollingSpeed();


	for (const PosAndQuaternion& point : points) {

		// ローカル座標取り出し
		const XMFLOAT3 localPos = point.position;
		const XMVECTOR quat = point.quaternion;

		// 頂点となる候補を追加
		AddTrailPoints(localPos, quat, rollingSpeed);
	}

	ID3D11Buffer* vertexBuffer = m_VertexBuffer.GetBuffer();
	ID3D11Buffer* indexBuffer = m_IndexBuffer.GetBuffer();

	vertexBuffer->Release();
	indexBuffer->Release();

	m_VertexBuffer.BufferReset();
	m_IndexBuffer.BufferReset();

	const unsigned int pointSize = static_cast<unsigned int>(m_TrailPoints.size());

	TrailMesh* trailMesh = dynamic_cast<TrailMesh*>(m_Mesh.get());
	if (trailMesh != nullptr) {

		std::vector<VERTEX_3D> vertices = trailMesh->CreateMeshVertices(pointSize * VertexNumSquare);
		std::vector<unsigned int> indices = trailMesh->CreateMeshIndices(pointSize * IndexNumSquare);

		m_VertexBuffer.Create(vertices);
		m_IndexBuffer.Create(indices);
	}

	const int vecSize = (int)m_SampleDivisions.size();

	if (vecSize > 0) {

		std::unordered_map<int, int> freq;
		int mode = m_SampleDivisions[0];
		int maxCount = 0;

		// 最頻値を求める
		for (int x : m_SampleDivisions) {

			// freq[x] はキーx がなければ{x,0}を作って返してくれる
			// 既に存在するキーであるならば、++freq[x]でインクリメントしてくれる

			int count = ++freq[x];	// インクリメントしてカウントを取得
			if (count > maxCount) {	// 最大数を更新
				maxCount = count;
				mode = x;
			}
		}

		m_AverageSamplingNum = mode/* + (vecSize / 4)*/;
	}
	else {
		m_AverageSamplingNum = 1;
	}
}

// 反転した際に頂点も左右反転させる
void TrailRenderComponent::InversionEvent() {

//	if (m_Inversion == RightLeft::LEFT) {

		for (TrailPoint& trail : m_TrailPoints) {
			trail.basePosition.x *= -1.0f;
			trail.tipPosition.x *= -1.0f;
		}

		m_Inversion = RightLeft::RIGHT;
//	}
}

void TrailRenderComponent::AddTrailPoints(const XMFLOAT3& center, const XMVECTOR& quaternion, const float trailSpeed) {

	if (!m_TrailPoints.empty()) {
		const TrailPoint& prev = m_TrailPoints.back();
		const XMVECTOR quatPrev = prev.localQuat;

		// SLERPで補間
		const float step = 0.1f;
		const XMVECTOR centerPrev = XMLoadFloat3(&prev.centerLocalPosition);
		const XMVECTOR centerNow = XMLoadFloat3(&center);

		// 前と次のセンターの距離を測って、一定距離ごとに補間点を追加
		const float length = XMVectorGetX(XMVector3Length(centerNow - centerPrev));
		const int div = std::max(1, (int)(length / step));

		for (int i = 1; i < div; ++i) {
			const float t = (float)i / div;

			// SLERPで回転補間
			const XMVECTOR quatInterp = XMQuaternionSlerp(quatPrev, quaternion, t);
			const XMMATRIX rotMtx = XMMatrixRotationQuaternion(quatInterp);
			const XMVECTOR right = rotMtx.r[0];

			float offsetValue = 0.0f;
			// 速度に応じてオフセット量を調整
			if (trailSpeed >= 1.0f) {
				offsetValue = trailSpeed * (trailSpeed * 0.001f);
			}

			// sinでずらし量を調整（πを掛けると中央が最大になる）
			float offsetScale = sinf(t * XM_PI) * offsetValue;
			XMVECTOR newCenter = XMVectorLerp(centerPrev, centerNow, t);
			const XMVECTOR offsetDir = XMVector3Normalize(newCenter - centerPrev);

			// 一旦見づらいので封印
			// 向きに合わせてオフセットでズラす
			newCenter += right * offsetScale;

			const XMVECTOR basePos = newCenter + right * m_BaseOffset;
			const XMVECTOR tipPos = newCenter + right * m_TipOffset;

			TrailPoint mid;
			XMStoreFloat3(&mid.centerLocalPosition, newCenter);
			XMStoreFloat3(&mid.basePosition, basePos);
			XMStoreFloat3(&mid.tipPosition, tipPos);
			mid.localQuat = quatInterp;
			mid.lifeTime = 0.0f;
			m_TrailPoints.push_back(mid);
		}

		m_SampleDivisions.push_back(div);
	}

	// 最後に今フレームの点を push
	const XMMATRIX rotMtx = XMMatrixRotationQuaternion(quaternion);
	const XMVECTOR right = rotMtx.r[0];
	const XMVECTOR centerVec = XMLoadFloat3(&center);

	TrailPoint newPoint;
	XMStoreFloat3(&newPoint.centerLocalPosition, centerVec);
	XMStoreFloat3(&newPoint.basePosition, centerVec + right * m_BaseOffset);
	XMStoreFloat3(&newPoint.tipPosition, centerVec + right * m_TipOffset);
	newPoint.localQuat = quaternion;
	newPoint.lifeTime = 0.0f;
	m_TrailPoints.push_back(newPoint);
}