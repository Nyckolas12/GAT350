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
#include "Triangle.h"

void InitScene(Scene& scene);
void InitScene01(Scene& scene, Camera& camera);

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	Time time;
	Input input;
	bool quit = false;

	Renderer* renderer = new Renderer;
	renderer->Initialize();
	renderer->CreateWindow("Ray Tracer", 800, 600);
	Framebuffer framebuffer(*renderer, renderer->GetWidth(), renderer->GetHeight());
	
	Camera camera{70.0f,framebuffer.m_width / (float)framebuffer.m_height};
	camera.SetView({ 0, 0, -10 },{0,0,0});

	SetBlendMode(BlendMode::Normal);
	
	
	Scene scene;
	InitScene(scene);
	scene.Update();
	

	framebuffer.Clear(ColorConvert(color4_t{ 128,128,128,255 }));
	scene.Render(framebuffer, camera, 10, 10);
	//tracer.Render(framebuffer, camera);

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
		
		
		
		renderer->CopyFramebuffer(framebuffer);
		framebuffer.Update();

		SDL_RenderPresent(renderer->m_renderer);

	}




	return 0;
}



void InitScene(Scene& scene)
{
	std::shared_ptr<Material> material = std::make_unique<Lambertian>(color3_t{ 1,0,1 });
	std::shared_ptr<Material> planematerial = std::make_unique<Lambertian>(color3_t{ 0.5f,1,0.5f });
	std::shared_ptr<Material> gray = std::make_shared<Lambertian>(color3_t{ 0.5f });
	std::shared_ptr<Material> red = std::make_shared<Lambertian>(color3_t{ HSVtoRGB(0.0f, 1.0f, 1.0f) });
	std::shared_ptr<Material> blue = std::make_shared<Dielectric>(color3_t{ 1, 1, 1 }, 1.33f);
	std::shared_ptr<Material> green = std::make_shared<Lambertian>(HSVtoRGB(120.0f, 1.0f, 1.0f));
	std::shared_ptr<Material> white = std::make_shared<Lambertian>(HSVtoRGB(0.0f, 0.0f, 1.0f));
	std::shared_ptr<Material> light = std::make_shared<Emissive>(HSVtoRGB(0.0f, 0.0f, 1.0f), 10.0f);

	std::vector<std::shared_ptr<Material>> materials = { red, blue };
	
	
	std::unique_ptr<Sphere> object = std::make_unique<Sphere>(glm::vec3{ 0 }, 2.0f, red);

	auto lightSource = std::make_unique<Sphere>(glm::vec3{ 0, 5, -5 }, 1.0f, white);
	scene.AddObject(std::move(lightSource));

	// Floor (white)
	auto WhitePlaneFloor = std::make_unique<Plane>(Transform{ glm::vec3{0, -5, 5}, glm::vec3{0,0,0 } }, white);
	scene.AddObject(std::move(WhitePlaneFloor));

	// Back wall (white)
	auto WhitePlaneBackWall = std::make_unique<Plane>(Transform{ glm::vec3{0, 0, 5 }, glm::vec3{ -90, 0, 0 }, glm::vec3{ 1 } }, white);
	scene.AddObject(std::move(WhitePlaneBackWall));

	// Left wall (red)
	auto RedPlaneLeft = std::make_unique<Plane>(Transform{ glm::vec3{ -10 , 0 , 0 }, glm::vec3{ 0, 0, 90 }, glm::vec3{ 1 } }, red);
	scene.AddObject(std::move(RedPlaneLeft));

	// Right wall (green)
	auto GreenPlaneRight = std::make_unique<Plane>(Transform{ glm::vec3{ 10 , 0 , 0 }, glm::vec3{ 0, 0, -90 }, glm::vec3{ 1 } }, green);
	scene.AddObject(std::move(GreenPlaneRight));

	// Ceiling light source
	auto Ceiling = std::make_unique<Plane>(Transform{ glm::vec3{0, 5, 5}, glm::vec3{180, 0, 0} }, white);
	scene.AddObject(std::move(Ceiling));
	
	auto CubeLight = std::make_unique<Model>(Transform{ { 0, 6, 0 }, glm::vec3{ 0 }, glm::vec3{ 2 } }, light);
	CubeLight->Load("models/cube.obj");
	scene.AddObject(std::move(CubeLight));

	auto model = std::make_unique<Model>(Transform{ { -2, -2, 1 }, { 0, 0, 0 }, glm::vec3{ 0.55f } }, std::make_shared<Metal>(HSVtoRGB(285, 0.85f, 0.75f), 0.8f));
	model->Load("models/teapot.obj");
	scene.AddObject(std::move(model));

}

void InitScene01(Scene& scene, Camera& camera)
{
	camera.SetFOV(20.0f);
	camera.SetView({ 13, 2, 3 }, { 0, 0, 0 });

	auto ground_material = std::make_shared<Lambertian>(color3_t(0.5f));
	scene.AddObject(std::make_unique<Plane>(Transform{ glm::vec3{ 0 } }, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = randomf();
			glm::vec3 center(a + 0.9 * randomf(), 0.2, b + 0.9 * randomf());

			if ((center - glm::vec3(4, 0.2, 0)).length() > 0.9) {
				std::shared_ptr<Material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = HSVtoRGB(randomf(0, 360), 1.0f, 1.0f);
					sphere_material = std::make_shared<Lambertian>(albedo);
					scene.AddObject(std::make_unique<Sphere>(Transform{ center }, 0.2f, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = HSVtoRGB(randomf(0, 360), 1.0f, 1.0f);
					auto fuzz = randomf(0.0f, 0.5f);
					sphere_material = std::make_shared<Metal>(albedo, fuzz);
					scene.AddObject(std::make_unique<Sphere>(Transform{ center }, 0.2f, sphere_material));
				}
				else {
					// glass
					sphere_material = std::make_shared<Dielectric>(color3_t{ 1 }, 1.5f);
					scene.AddObject(std::make_unique<Sphere>(Transform{ center }, 0.2f, sphere_material));
				}
			}
		}
	}

	auto material1 = std::make_shared<Dielectric>(color3_t{ 1 }, 1.5f);
	scene.AddObject(std::make_unique<Sphere>(Transform{ glm::vec3{ 0, 1, 0 } }, 1.0f, material1));

	auto material2 = std::make_shared<Lambertian>(color3_t(0.4f, 0.2f, 0.1f));
	scene.AddObject(std::make_unique<Sphere>(Transform{ glm::vec3{ -4, 1, 0 } }, 1.0f, material2));

	auto material3 = std::make_shared<Metal>(color3_t(0.7f, 0.6f, 0.5f), 0.0f);
	scene.AddObject(std::make_unique<Sphere>(Transform{ glm::vec3{ 4, 1, 0 } }, 1.0f, material3));
}
