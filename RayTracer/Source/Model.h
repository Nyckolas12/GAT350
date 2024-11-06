#pragma once
#include "Color.h"

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "SceneObject.h"

using Vertex_t = glm::vec3;
using Verticies_t = std::vector<Vertex_t>;

class Model :public SceneObject
{
public:
	Model(std::shared_ptr<Material> material) : SceneObject{ material } {}
	Model(const Transform& transform,std::shared_ptr<Material> material) : SceneObject{ transform, material } {}
	Model(const Verticies_t& verticies, std::shared_ptr<Material> material) : SceneObject{ material }, m_verticies{ verticies } {}


	void Update() override;
	void Draw(class Framebuffer& framebuffer, const glm::mat4& model, const class Camera& camera);
	bool Load(const std::string& filename);
	bool Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDistance, float maxDistance) override;

private:
	Verticies_t m_verticies;
	Verticies_t m_local_verticies;
	glm::vec3 m_center{ 0 };
	float m_radius{ 0 };
};