#include "Renderer.h"
#include "Framebuffer.h"
#include "Image.h"
#include "PostProcess.h"
#include "Color.h"
#include "Transform.h"
#include "ETime.h"
#include "Input.h"
#include "Model.h"
#include "Camera.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include <iostream>
#include "Actor.h"

int main(int argc, char* argv[])
{
	Time time;
	Input input;
	input.Initialize();

	Renderer* renderer = new Renderer;
	renderer->Initialize();
	renderer->CreateWindow("Game Engine", 800, 600);
	Framebuffer framebuffer(*renderer, 800, 600);
	
	Image image;
	image.Load("scenic.jpg");

	Image imageAlt;
	imageAlt.Load("colors.png");

	SetBlendMode(BlendMode::Normal);
	Camera camera(renderer->GetWidth(), renderer->GetHeight());
	camera.SetView(glm::vec3{ 0, 40, -50 }, glm::vec3{ 1 });
	camera.SetProjection(120.0f, 800.0f / 600.0f, 0.1f, 200.0f);
	Transform cameraTransform{ {2, -3, -17} };
	//cameraTransform.rotation = { 90,90,90 };
	
	//shader
	VertexShader::uniforms.view = camera.GetView();
	VertexShader::uniforms.projection = camera.GetProjection();
	//VertexShader::uniforms.ambient = color3_t{ 0.1f };
	//light
	VertexShader::uniforms.light.position = glm::vec3{ -40, 40, -5 };
	VertexShader::uniforms.light.direction = glm::vec3{ 10, -100, 0 }; // light pointing down
	VertexShader::uniforms.light.color = color3_t{ 1,0,1 };

	Shader::framebuffer = &framebuffer;

	bool quit = false;

	std::shared_ptr<Model> teacupModel = std::make_shared<Model>();
	teacupModel->Load("models/bunny.obj");
	teacupModel->SetColor({ 0,0,1,1 });
	Transform teacupTransform{ {2, 1, 1}, glm::vec3{20, 0, -270}, glm::vec3{2} };
	std::vector<std::unique_ptr<Actor>> actors;
	auto teacupActor = std::make_unique<Actor>(teacupTransform, teacupModel);
	actors.push_back(std::move(teacupActor));
	for (auto& actor : actors)
	{
		actor->GetTransform().rotation.y += time.GetDeltaTime() * 90;
		actor->Draw();
	}



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
	
		input.Update();
		time.Tick();

		framebuffer.Clear(color_t{ 128,128,128,255 });
		for (int i = 0; i < 10; i++)
		{
			int x = rand() % framebuffer.m_width;
			int y = rand() % framebuffer.m_height;
			int x2 = rand() % framebuffer.m_width;
			int x3 = rand() % framebuffer.m_width;
			int y2 = rand() % framebuffer.m_height;
			int y3 = rand() % framebuffer.m_height;
			//framebuffer.DrawPoint(x, y, { 55,100,156,255 });
			//framebuffer.DrawLine(x, y, x2, y2, { 25,45,67 });
			//framebuffer.DrawRect(x, y, 40, 40, { 255,255,255 });
			//framebuffer.DrawTrianlge(x, y, x2, y2, x3, y3, { 25,200,76,255 });
			//framebuffer.DrawCircle(x,y , 30, { 55,100,156,255 });
			
			
			//framebuffer.DrawImage(x, y, image);
			//framebuffer.DrawImage(x2, y2, image);
		}

#pragma region PostProcess
		SetBlendMode(BlendMode::Normal);
		//framebuffer.DrawImage(50, 100, image);
		//SetBlendMode(BlendMode::Alpha);
		//framebuffer.DrawImage(50, 100, imageAlt);
		int mx, my;
		SDL_GetMouseState(&mx, &my);
		//framebuffer.DrawLinearCurve(100, 100, 200, 200, { 255,255,0,255 });
		//framebuffer.DrawQuadraticCurve(100, 200, 200, 100, 300, 200, { 255,0,0,255 });
		//framebuffer.DrawCubicCurve(300, 500, 300, 200, mx, my, 600, 500, { 255,0,0,255 });

		//PostProcess::Invert(framebuffer.m_buffer);
		//PostProcess::Monochrome(framebuffer.m_buffer);
		//PostProcess::Brightness(framebuffer.m_buffer,100);
		//PostProcess::Posterize(framebuffer.m_buffer, 6);
		//PostProcess::Noise(framebuffer.m_buffer, 80);
		//PostProcess::ColorBalance(framebuffer.m_buffer, 150, -50, -50);
		//PostProcess::Threshold(framebuffer.m_buffer, 150);
		//PostProcess::Alpha(imageAlt.m_buffer, 155);
		//PostProcess::BoxBlur(framebuPostProcess::Alpha(imageAlt.m_buffer, 255);ffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
		//PostProcess::GaussianBlur(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
		//PostProcess::Sharpen(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
		//PostProcess::Sharpen(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
		//PostProcess::Edge(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height, 10);
		//PostProcess::Emboss(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);

#pragma endregion
		glm::vec3 direction{ 0 };
		int rotation{ 0 };

		if (input.GetKeyDown(SDL_SCANCODE_RIGHT))
		{
			direction.x = 1;
			rotation = 1;
		}
		if (input.GetKeyDown(SDL_SCANCODE_LEFT)) direction.x = -1;
		if (input.GetKeyDown(SDL_SCANCODE_UP)) direction.y = -1;
		if (input.GetKeyDown(SDL_SCANCODE_DOWN)) direction.y = 1;

		if (input.GetKeyDown(SDL_SCANCODE_W)) direction.z = -1;
		if (input.GetKeyDown(SDL_SCANCODE_S)) direction.z = 1;
		if (input.GetKeyDown(SDL_SCANCODE_Q)) rotation = 1;
		if (input.GetKeyDown(SDL_SCANCODE_E)) rotation = -1;


		glm::vec3 offset = cameraTransform.GetMatrix() * glm::vec4{ direction, 0 };
		cameraTransform.position += offset * 100.0f * time.GetDeltaTime();
		cameraTransform.rotation.y = input.GetMousePosition().x * 0.1f;
		cameraTransform.rotation.x = input.GetMousePosition().y * 0.1f;


		//cameraTransform.position += direction * 100.0f * time.GetDeltaTime();
		camera.SetView(cameraTransform.position, cameraTransform.position + cameraTransform.GetForward());
		VertexShader::uniforms.view = camera.GetView();

		teacupModel->Draw();
		
		renderer->CopyFramebuffer(framebuffer);
		framebuffer.Update();

		SDL_RenderPresent(renderer->m_renderer);

	}




	return 0;
}