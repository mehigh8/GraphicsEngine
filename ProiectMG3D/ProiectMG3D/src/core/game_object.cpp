#include "core/game_object.h"

GameObject::GameObject(std::string objectID)
{
	this->objectID = std::move(objectID);

	this->position = glm::vec3(0.0f);
	this->rotation = glm::vec3(0.0f);
	this->scale = glm::vec3(1.0f);

	this->model = glm::mat4(1.0f);
}

void GameObject::CalculateModelMatrix()
{
	model = glm::mat4(1.0f);

	model = glm::translate(model, position);
	model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, scale);
}

std::string GameObject::GetObjectID()
{
	return objectID;
}
