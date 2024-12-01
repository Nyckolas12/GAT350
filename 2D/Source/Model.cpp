#include "Model.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
/// <summary>
/// Convert point from model space to world space
/// </summary>
/// <param name="framebuffer">The buffer we are drawing to</param>
/// <param name="model">the model matrix that holds the points in model space</param>
void Model::Draw()
{
	Shader::Draw(m_vb);
}

bool Model::Load(const std::string& filename)
{

	std::ifstream stream(filename);
	if (!stream.is_open()) {
		std::cerr << "Error opening ifstream" << std::endl;
		return false;
	}
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	
	std::string line;
	while (std::getline(stream, line)) {
		//read in vertex positions
		if (line.substr(0,2).find("v ") == 0) {
			std::istringstream sstream{ line.substr(2) };
			glm::vec3 position;
			sstream >> position.x;
			sstream >> position.y;
			sstream >> position.z;
			vertices.push_back(position);
		}
		else if ((line.substr(0,3).find("vn ") == 0)) {
			// read in vertext normal
			std::istringstream sstream{ line.substr(3) };
			glm::vec3 normal;
			sstream >>normal.x;
			sstream >>normal.y;
			sstream >>normal.z;
			normals.push_back(normal);
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
				if (index[0] && index[1]) {
					
					vertex_t vertex;
					vertex.position = vertices[index[0] - 1];
					vertex.normal = (index[2]) ? normals[index[2] - 1] : glm::vec3{ 1 };
					m_vb.push_back(vertex);
				}
			}
		}
	}
	stream.close();
	return true;
}
