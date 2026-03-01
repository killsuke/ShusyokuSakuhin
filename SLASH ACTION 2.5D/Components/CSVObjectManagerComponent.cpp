#include "CSVObjectManagerComponent.h"

CSVObjectManagerComponent::CSVObjectManagerComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("CSV_OBJECT_MANAGER"); // É\Å[Égî‘çÜÇê›íË
}

void CSVObjectManagerComponent::Update() {


}
