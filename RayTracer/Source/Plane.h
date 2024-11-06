#pragma once
#include"SceneObject.h"
#include "Ray.h"
#include <glm/glm.hpp>
class Plane : public SceneObject
{
public:
	Plane() = default;
	Plane(const Transform& transform, std::shared_ptr<Material> material) : SceneObject(transform, material) {}
	Plane(const glm::vec3& center, const glm::vec3& normal, std::shared_ptr<Material> material) :  SceneObject{ material }, m_center{ center }, m_normal{ normal } {}
	

	bool Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDistance, float maxDistance) override;
	static bool Raycast(const ray_t& ray, const glm::vec3& point, const glm::vec3& normal, float minDistance, float maxDistance, float& t);

private:
	glm::vec3 m_center{ 0 };
	glm::vec3 m_normal{ 0 };
		
};