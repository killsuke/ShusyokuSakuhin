#pragma once
#include <SimpleMath.h>
#include "XMFLOAT_Helper.h"

DirectX::SimpleMath::Vector3 QuaternionToEulerRad(const DirectX::SimpleMath::Quaternion& qf);

DirectX::SimpleMath::Vector3 QuaternionToEulerDeg(const DirectX::SimpleMath::Quaternion& qf);

DirectX::XMFLOAT3 QuaternionToEulerRad(const DirectX::XMVECTOR& q);

DirectX::XMFLOAT3 QuaternionToEulerDeg(const DirectX::XMVECTOR& q);

void DecomposeMatrix(const DirectX::XMMATRIX& matrix, DirectX::XMVECTOR& outScale, DirectX::XMVECTOR& outRotation, DirectX::XMVECTOR& outTranslation);