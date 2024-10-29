#include "Tracer.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Scene.h"
#include "Color.h"

color3_t Tracer::Trace(Scene& scene, const ray_t& ray, float minDistance, float maxDistance, int depth)
{
	if (depth == 0) return color3_t{ 0 };
	raycastHit_t raycastHit;
	float closestDistance = maxDistance;
	bool isHit = false;
	color3_t color{ 0 };
	//check for object hit in scene	
	for (auto& object : scene.m_objects)
	{
		if (object->Hit(ray,raycastHit,minDistance,closestDistance))
		{
			isHit = true;
			closestDistance = raycastHit.distance;
			
		}
	}

	if (isHit)
	{
		color3_t attenuation;
		ray_t scatter;
		if (raycastHit.material.lock()->Scatter(ray, raycastHit, attenuation, scatter))
		{
			return attenuation * Trace(scene, scatter, minDistance, maxDistance, depth - 1);
			/*return raycastHit.material.lock()->GetColor();*/
		}
		else 
		{
			return raycastHit.material.lock()->GetEmissive();
		}
	}

	//sky
	glm::vec3 direction = glm::normalize(ray.direction);
	float t = (direction.y + 1) * 0.5f;
	 color = Lerp(color3_t{ 1,1,1 }, color3_t{ 0,0.5f,0.75f }, t);

	

	return color;
}
