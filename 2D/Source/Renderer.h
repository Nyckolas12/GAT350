#pragma once
#include <string>
#include<SDL.h>
class Renderer
{
public:
	Renderer() = default;
	bool Initialize();
	bool CreateWindow();
	void BeginFrame();
	void EndFrame();

private:
	SDL_Renderer* m_renderer{nullptr};
};