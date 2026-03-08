// =============================================================================
// 誰に影を出すかを管理するクラス
// プレイヤーは絶対に影を出すようにする
// ただし、敵は最大で31体までしか影を出さないようにする
// =============================================================================

#include "System/DirectXRender.h"
#include "ShadowManager.h"

using namespace DirectX;

void ShadowManager::Update()
{
    // 敵とプレイヤーを取得
    std::vector<GameObject*> enemyObjects = GameObjectManager::GameObjectFindTag("Enemy");
    GameObject* playerObject = GameObjectManager::GameObjectFindName("Player");

    // 無効スロット用の初期値（描画されない）
    ShadowData invalid;
    invalid.objectPos = XMFLOAT3(99999.0f, 99999.0f, 99999.0f);
    invalid.shadowRadius = 0.0f;

    // 全スロットを無効値で初期化
    std::fill(m_ShadowBuffers.begin(), m_ShadowBuffers.end(), invalid);

    int count = 0;

    // ================================
    // 0 番目：プレイヤーの影を必ず入れる
    // ================================
    if (playerObject != nullptr)
    {
        TransformComponent* playerTrans = playerObject->GetComponent<TransformComponent>();
        if (playerTrans != nullptr)
        {
            ShadowData buf;
            buf.objectPos = playerTrans->GetPosition();
            buf.shadowRadius = playerTrans->GetScale().x * SHADOW_RADIUS_MULTIPLIER_PLAYER;

            m_ShadowBuffers[0] = buf;
            count = 1; // 敵は 1 番目から
        }
    }

    // ================================
    // 敵の影を 1 番目以降に詰める
    // ================================
    const size_t enemyCount = std::min(enemyObjects.size(), static_cast<size_t>(ENEMY_MAX));

    for (size_t i = 0; i < enemyCount; ++i)
    {
		ActiveState state = enemyObjects[i]->GetActiveState();
        if (state == ActiveState::ALL_STOP || state == ActiveState::DRAW_STOP) {
            continue; // 描画停止状態の敵はスキップ
		}

        TransformComponent* enemyTrans = enemyObjects[i]->GetComponent<TransformComponent>();

        ShadowData buf;
        buf.objectPos = enemyTrans->GetPosition();
        buf.shadowRadius = enemyTrans->GetScale().x * SHADOW_RADIUS_MULTIPLIER_ENEMY;

        m_ShadowBuffers[count] = buf;
        ++count;
    }

    // ================================
    // シェーダーに渡す影の数
    // ================================
    m_ShadowCount = count;

    ID3D11DeviceContext* context = DirectXRender::GetDeviceContext();

	ID3D11Buffer* shadowBuffer = DirectXRender::GetShadowBuffer();

	ShadowBuffer bufferData;
	bufferData.shadowCount = m_ShadowCount;
	std::copy(m_ShadowBuffers.begin(), m_ShadowBuffers.end(), bufferData.shadowData.begin());

	context->UpdateSubresource(shadowBuffer, 0, nullptr, &bufferData, 0, 0);
}