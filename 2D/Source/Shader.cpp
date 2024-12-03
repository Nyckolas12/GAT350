#include "Shader.h"
#include "Framebuffer.h"
#include "Rasterizer.h"

Framebuffer* Shader::framebuffer{ nullptr };
Shader::eFrontFace Shader::front_face = Shader::CW;   // Default winding order
Shader::eCullMode Shader::cull_mode = Shader::NONE;     // Default cull mode

void Shader::Draw(const vertexbuffer_t& vb)
{
	// vertex shader
	std::vector<vertex_output_t> overtices;
	overtices.reserve(vb.size());
	for (auto& vertex : vb)
	{
		vertex_output_t overtex;
		VertexShader::Process(vertex, overtex);
		overtices.push_back(overtex);
	}

	// primitive assembly (triangle)
	for (int i = 0; i < overtices.size(); i += 3)
	{
		vertex_output_t& v0 = overtices[i];
		vertex_output_t& v1 = overtices[i + 1];
		vertex_output_t& v2 = overtices[i + 2];

		glm::vec2 s0, s1, s2;
		if (!ToScreen(v0, s0)) continue;
		if (!ToScreen(v1, s1)) continue;
		if (!ToScreen(v2, s2)) continue;


		// Compute signed area (cross product)
		float z = cross(s1 - s0, s2 - s0);
		if (std::fabs(z) < std::numeric_limits<float>::epsilon()) continue;
		//Determines if the triangle should be culled based on winding order and cull mode
		//bool isFrontFacing = (z > 0 && front_face == CCW) || (z < 0 && front_face == CW);
		
		// cull faces
		switch (cull_mode)
		{
		case FRONT:
			if (front_face == CCW && z > 0) continue; // Skip front-facing triangles
			if (front_face == CW && z < 0) continue; // Skip front-facing triangles
			break;
		case BACK:
			if (front_face == CCW && z < 0) continue; // Skip back-facing triangles
			if (front_face == CW && z > 0) continue; // Skip back-facing triangles
			break;
		case NONE:
			// No culling, render all faces
			break;
		}

		// rasterization
		Rasterizer::Triangle(*framebuffer,s0,s1,s2,v0,v1,v2);
		//framebuffer->DrawTrianlge(s0.x, s0.y, s1.x, s1.y, s2.x, s2.y, { 255,255,255,255 });
	}
}

bool Shader::ToScreen(const vertex_output_t& vertex, glm::vec2& screen)
{
	// prevent / 0
	if (vertex.position.w == 0) return false;

	// convert projection space to ndc (-1 <-> 1)
	glm::vec3 ndc = vertex.position / vertex.position.w;

	// don't draw if outside near and far clip plane
	if (ndc.z < -1 || ndc.z > 1) return false;

	screen.x = (ndc.x + 1) * (framebuffer->m_width * 0.5f);
	screen.y = (1 - ndc.y) * (framebuffer->m_height * 0.5f);

	return true;
}