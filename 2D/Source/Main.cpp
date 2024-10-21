#include "Renderer.h"
#include "Framebuffer.h"
#include "PostProcess.h"
#include "Image.h"
#include "Color.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[])
{

	Renderer* renderer = new Renderer;
	renderer->Initialize();
	renderer->CreateWindow("Game Engine", 800, 600);
	Framebuffer framebuffer(*renderer, 800, 600);
	
	Image image;
	image.Load("scenic.jpg");

	Image imageAlt;
	imageAlt.Load("colors.png");
	
	

	bool quit = false;

	//OpenGl Math
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
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
		framebuffer.Clear(color_t{ 255,255,255,255 });
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
		SetBlendMode(BlendMode::Normal);
		framebuffer.DrawImage(50, 100, image);
		SetBlendMode(BlendMode::Alpha);
		framebuffer.DrawImage(50, 100, imageAlt);
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
		PostProcess::Alpha(imageAlt.m_buffer, 155);
		//PostProcess::BoxBlur(framebuPostProcess::Alpha(imageAlt.m_buffer, 255);ffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
		//PostProcess::GaussianBlur(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
		//PostProcess::Sharpen(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
		//PostProcess::Sharpen(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
		//PostProcess::Edge(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height, 10);
		//PostProcess::Emboss(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
		
		renderer->CopyFramebuffer(framebuffer);
		framebuffer.Update();

		SDL_RenderPresent(renderer->m_renderer);

	}




	return 0;
}