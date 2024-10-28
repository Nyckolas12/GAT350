#include "Renderer.h"
#include "Framebuffer.h"
#include "Image.h"
#include "PostProcess.h"
#include "Color.h"
#include "Transform.h"
#include "ETime.h"
#include "Input.h"
#include "Tracer.h"
#include "Model.h"
#include "Sphere.h"
#include "Plane.h"
#include "Scene.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include <iostream>
#include "Random.h"

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	Time time;
	Input input;

	Renderer* renderer = new Renderer;
	renderer->Initialize();
	renderer->CreateWindow("Ray Tracer", 800, 600);
	Framebuffer framebuffer(*renderer, renderer->GetWidth(), renderer->GetHeight());
	
	Camera camera{70.0f,framebuffer.m_width / (float)framebuffer.m_height};
	camera.SetView({0,0,-20},{0,0,0});

	SetBlendMode(BlendMode::Normal);
	
	
	Scene scene;
	std::shared_ptr<Material> material = std::make_unique<Lambertian>(color3_t{ 1,0,1 });
	std::shared_ptr<Material> planematerial = std::make_unique<Lambertian>(color3_t{ 0.5f,1,0.5f });
	std::shared_ptr<Material> gray = std::make_shared<Lambertian>(color3_t{ 0.5f });
	std::shared_ptr<Material> red = std::make_shared<Metal>(color3_t{ 1, 0, 0 }, 0.3f);
	std::shared_ptr<Material> blue = std::make_shared<Metal>(color3_t{ 0, 0, 1 }, 0.1f);
	std::shared_ptr<Material> green = std::make_shared<Metal>(color3_t{ 0, 1, 0 }, 0.3f);
	
	std::vector<std::shared_ptr<Material>> materials = { red, blue };



	std::unique_ptr<Sphere> object = std::make_unique<Sphere>(glm::vec3{ 0 }, 2.0f, material);
	auto plane = std::make_unique<Plane>(glm::vec3{ 0, -10, 0 }, glm::vec3{ 0, 2.0f, 0 }, planematerial);
	scene.AddObject(std::move(object));
	scene.AddObject(std::move(plane));


	// Function to select a random material from the vector
	auto getRandomMaterial = [&materials]() -> std::shared_ptr<Material> {int index = static_cast<int>(randomf(0, materials.size())); return materials[index];};

	for (int i = 0; i < 10; ++i) {
		glm::vec3 random_position = random(glm::vec3{ -10.0f, -10.0f, -10.0f }, glm::vec3{ 10.0f, 10.0f, 10.0f });
		float random_radius = randomf(0.5f, 2.0f);  // Random radius between 0.5 and 2.0
		auto random_material = getRandomMaterial(); // Get a random material

		
		auto sphere = std::make_unique<Sphere>(random_position, random_radius, random_material);
		scene.AddObject(std::move(sphere));
	}

	bool quit = false;



	

	
	


	//Model teacupModel;
	//Transform teacupTransform{ {20, 1, 1}, glm::vec3{0, 0, 180}, glm::vec3{3} };
	//teacupModel.Load("teapot.obj");
	//teacupModel.SetColor({ 0, 0, 255, 255 });

	while (!quit)
	{
	
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = true;
			}
		}
		
		framebuffer.Clear(ColorConvert(color4_t{ 128,128,128,255 }));

		
		
		scene.Render(framebuffer, camera);
		//tracer.Render(framebuffer, camera);
		
		
		renderer->CopyFramebuffer(framebuffer);
		framebuffer.Update();

		SDL_RenderPresent(renderer->m_renderer);

	}




	return 0;
}