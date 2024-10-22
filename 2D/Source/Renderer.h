#pragma once
#include <string>
#include<SDL.h>
class Renderer
{
public:
	Renderer() = default;
	bool Initialize();
	bool CreateWindow(std::string title, int width, int height);
	void CopyFramebuffer(const class Framebuffer& framebuffer);
	void operator = (const class Framebuffer& framebuffer);
	void BeginFrame();
	void EndFrame();
	friend class Framebuffer;
	SDL_Renderer* m_renderer{nullptr};
	int GetHeight() { return m_height; }
	int GetWidth() { return m_width; }

private:
	int m_width = 0;
	int m_height = 0;
	SDL_Window* m_window{ nullptr };
};