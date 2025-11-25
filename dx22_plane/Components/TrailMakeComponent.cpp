#include "TrailMakeComponent.h"
#include "Transform.h"
#include "DirectXRender.h"
#include "Mesh/TrailMesh.h"
#include <vector>
#include <iostream>

namespace {
	constexpr float DeltaTime = 0.016f;
	constexpr int VertexNumSquare = 4;
	constexpr int IndexNumSquare = 6;
}

TrailMakeComponent::TrailMakeComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("TRAILMAKE"); // ƒ\[ƒg”Ô†‚ðÝ’è
}

void TrailMakeComponent::Update() {

	if(m_TrailRenderComponent == nullptr) {
		return;
	}
	m_TrailRenderComponent->TrailCountUp();

//	std::cout << "TrailMakeComponent" << std::endl;
}