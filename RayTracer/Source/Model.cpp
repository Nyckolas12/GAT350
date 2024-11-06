#include "Model.h"
#include "Framebuffer.h"
#include "Camera.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Triangle.h"
#include "Sphere.h"
void Model::Update()
{
	
	for (size_t i = 0; i < m_local_verticies.size(); i++)
	{
		m_verticies[i] = m_transform * glm::vec4{ m_local_verticies[i], 1 };
	}

	m_center = glm::vec3{ 0 };
	for (auto& vertex : m_verticies)
	{
		m_center += vertex;

	}
	m_center /= (float)m_verticies.size();

	m_radius = 0;
	for (auto& vertex : m_verticies)
	{
		float radius = glm::length(vertex - m_center);
		m_radius = glm::max(radius, m_radius);
	}
}
/// <summary>
/// Convert point from model space to world space
/// </summary>
/// <param name="framebuffer">The buffer we are drawing to</param>
/// <param name="model">the model matrix that holds the points in model space</param>
void Model::Draw(Framebuffer& framebuffer, const glm::mat4& model, const Camera& camera)
{
	
}

bool Model::Load(const std::string& filename)
{

	std::ifstream stream(filename);
	if (!stream.is_open()) {
		std::cerr << "Error opening ifstream" << std::endl;
		return false;
	}
	Verticies_t vertices;
	
	std::string line;
	while (std::getline(stream, line)) {
		//read in vertex positions
		if (line.substr(0, 2) == "v ") {
			std::istringstream sstream{ line.substr(2) };
			glm::vec3 position;
			sstream >> position.x;
			sstream >> position.y;
			sstream >> position.z;
			vertices.push_back(position);
		}
		else if (line.substr(0, 2) == "f ") {
			std::istringstream sstream{ line.substr(2) };
			std::string str;
			while (std::getline(sstream, str, ' ')) {
				std::istringstream sstream(str);
				std::string indexString;

				size_t i = 0;
				unsigned int index[3] = { 0,0,0 };
				while (std::getline(sstream, indexString, '/')) {
					if (!indexString.empty()) {
						std::istringstream indexStream{ indexString };
						//stream indexstream into index[i]
						indexStream >> index[i];
					}
					i++;
				}
				if (index[0] != 0) {
					glm::vec3 position = vertices[index[0] - 1];
					m_local_verticies.push_back(position);
				}
			}
		}
	}
	m_verticies.resize(m_local_verticies.size());
	stream.close();
	return true;
}

bool Model::Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDistance, float maxDistance)
{
	// check cast ray with mesh triangles 
	float t;
	if (!Sphere::Raycast(ray, m_center, m_radius, minDistance, maxDistance, t))
	{
		return false;
	}


	for (size_t i = 0; i < m_verticies.size(); i += 3)
	{
		float t;
		if (Triangle::Raycast(ray, m_verticies[i], m_verticies[i + 1], m_verticies[i + 2], minDistance, maxDistance, t))
		{
			raycastHit.distance = t;
			raycastHit.point = ray.At(t);
			glm::vec3 edge1 = m_verticies[i + 1] - m_verticies[i];
			glm::vec3 edge2 = m_verticies[i + 2] - m_verticies[i];

			raycastHit.normal = glm::normalize(Cross(edge1, edge2));
			raycastHit.material = GetMaterial();
			return true;
		}
		
	}

	return false;
}
