#include "TrailMakeComponent.h"
#include "Transform.h"
#include "DirectXRender.h"
#include "Mesh/TrailMesh.h"
#include <vector>

namespace {
	constexpr float DeltaTime = 0.016f;
	constexpr int VertexNumSquare = 4;
	constexpr int IndexNumSquare = 6;
}

TrailMakeComponent::TrailMakeComponent(GameObject& obj) : Component(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("TRAILMAKE"); // É\Å[Égî‘çÜÇê›íË
}

void TrailMakeComponent::Update() {



}