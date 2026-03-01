#pragma once
#include <DirectXMath.h>
#include "Helper/XMFLOAT_Helper.h"
#include <math.h>
#include <limits.h>
#include <stdbool.h>

DirectX::XMFLOAT3 QuaternionToEulerRad(const DirectX::XMVECTOR& q);

DirectX::XMFLOAT3 QuaternionToEulerDeg(const DirectX::XMVECTOR& q);

void DecomposeMatrix(const DirectX::XMMATRIX& matrix, DirectX::XMVECTOR& outScale, DirectX::XMVECTOR& outRotation, DirectX::XMVECTOR& outTranslation);

bool FloatToInt(const float f,int* out);