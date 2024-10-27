#include "Model.h"
#include "Framebuffer.h"
#include "Camera.h"
#include <iostream>
#include <fstream>
#include <sstream>
/// <summary>
/// Convert point from model space to world space
/// </summary>
/// <param name="framebuffer">The buffer we are drawing to</param>
/// <param name="model">the model matrix that holds the points in model space</param>
void Model::Draw(Framebuffer& framebuffer, const glm::mat4& model, const Camera& camera)
{
	for (int i = 0; i < m_verticies.size() - 2; i += 3)
	{
		//
		Vertex_t p1 = model * glm::vec4{ m_verticies[i], 1 };
		Vertex_t p2 = model * glm::vec4{ m_verticies[i + 1], 1 };
		Vertex_t p3 = model * glm::vec4{ m_verticies[i + 2], 1 };

		p1 = camera.ModelToView(p1);
		p2 = camera.ModelToView(p2);
		p3 = camera.ModelToView(p3);

		glm::ivec2 s1 = camera.ToScreen(p1);
		glm::ivec2 s2 = camera.ToScreen(p2);
		glm::ivec2 s3 = camera.ToScreen(p3);

		if (s1.x == -1 || s1.y == -1 || s2.x == -1 || s2.y == -1 || s3.x == -1 || s3.y == -1) {
			continue;
		}

		framebuffer.DrawTrianlge(s1.x, s1.y, s2.x, s2.y, s3.x, s3.y, m_color);
	}
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
					m_verticies.push_back(position);
				}
			}
		}
	}
	stream.close();
	return true;
}
