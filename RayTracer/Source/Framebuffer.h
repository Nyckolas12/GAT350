#pragma once
#include "SDL.h"
#include <vector>
using color_t = SDL_Color;
class Framebuffer
{
public:
	Framebuffer(const class Renderer& renderer, int width, int height);
	~Framebuffer();

	void Update();
	void Clear(const color_t& color);

	void DrawPoint(int x, int y, const color_t& color);
	void DrawPointClip(int x, int y, const color_t& color);
	void DrawRect(int x, int y, int width, int height, const color_t& color);
	void DrawLineSlope(int x1, int y1, int x2, int y2, const color_t& color);
	void DrawLine(int x1, int y1, int x2, int y2, const color_t& color);
	void DrawTrianlge(int x1, int y1, int x2, int y2, int x3, int y3, const color_t& color);
	void Drawpixel(int xc, int yc, int x,int y, const color_t& color);
	void DrawCircle(int xc, int yc, int r, const color_t& color);

	int ClippingRegionCode(int x, int y);
	void CohenSutherlandClipDetection(int& x1, int& y1, int& x2, int& y2);

	void DrawLinearCurve(int x1, int y1, int x2, int y2, const color_t& color);
	void DrawQuadraticCurve(int x1, int y1, int x2, int y2,int x3,int y3, const color_t& color);
	void DrawCubicCurve(int x1, int y1, int x2, int y2,int x3,int y3, int x4, int y4, const color_t& color);
	void DrawImage(int x, int y, const class Image& image);

	friend class Renderer;

public:
	int m_width{ 0 };
	int m_height{ 0 };
	int m_pitch{ 0 };

	SDL_Texture* m_texture{ nullptr };
	std::vector<color_t> m_buffer;
};