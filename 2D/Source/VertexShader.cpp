#include "VertexShader.h"

VertexShader::uniforms_t VertexShader::uniforms =
{
	glm::mat4{1}, //model
	glm::mat4{1}, // view
	glm::mat4{1}, //projection
	color3_t{1 }
};

void VertexShader::Process(const vertex_t& ivertex, vertex_output_t& overtex)
{
	glm::mat4 mvp = uniforms.projection * uniforms.view * uniforms.model;
	overtex.position = mvp * glm::vec4{ ivertex.position, 1 };
	glm::mat4 mv = uniforms.view * uniforms.model;
	overtex.normal = glm::normalize(glm::mat3{ mv } * ivertex.normal);
	glm::vec3 vposition = mv * glm::vec4{ ivertex.position,1 };

	
	//Point Light Calculation 
	glm::vec3 light_position_view = glm::vec3(uniforms.view * glm::vec4{ uniforms.light.position, 1 });
	glm::vec3 light_dir_point = glm::normalize(light_position_view - vposition);
	float intensity_point = std::max(0.0f, glm::dot(light_dir_point, overtex.normal));
	color3_t diffuse_point = uniforms.light.color * intensity_point;

	// Directional Light Calculation 
	glm::vec3 light_dir_view = glm::normalize(glm::mat3(uniforms.view) * glm::vec3(-uniforms.light.direction)); 
	float intensity_directional = std::max(0.0f, glm::dot(light_dir_view, overtex.normal));
	color3_t diffuse_directional = uniforms.light.color * intensity_directional;

	/*glm::vec3 light_position{ 10, 10, -10 };
	light_position = uniforms.view * glm::vec4{ light_position, 1 };
	glm::vec3 light_dir = glm::normalize(light_position - vposition);

	float intensity = std::max(0.0f,glm::dot(light_dir, overtex.normal));
	color3_t light_color{ 1,0.5f,0.3f };
	color3_t diffuse = uniforms.light.color * intensity;*/
	overtex.color = uniforms.ambient + diffuse_point + diffuse_directional;
}
