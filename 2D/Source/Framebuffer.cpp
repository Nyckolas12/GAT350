#include "Framebuffer.h"
#include "Renderer.h"
#include <iostream>
Framebuffer::Framebuffer(const Renderer& renderer, int width, int height)
{
	m_width = width;
	m_height = height;
	m_pitch = width * sizeof(color_t);
	m_texture = SDL_CreateTexture(renderer.m_renderer, SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_STREAMING,width, height);
	if (!m_texture)
	{
		std::cerr << "Error creating SDL Framebuffer: " << SDL_GetError() << std::endl;
	}

	m_buffer.resize(m_width * m_height);
}

Framebuffer::~Framebuffer()
{
	SDL_DestroyTexture(m_texture);

}

void Framebuffer::Update()
{
	SDL_UpdateTexture(m_texture, NULL, m_buffer.data(), m_pitch);
}

void Framebuffer::Clear(const color_t& color)
{
	std::fill(m_buffer.begin(), m_buffer.end(), color);
}

void Framebuffer::DrawPoint(int x, int y, const color_t& color)
{
	if (x < 0 ||x > m_width || y < 0 || y >= m_height) return;
	m_buffer[x+y * m_width] = color;
}

void Framebuffer::DrawRect(int x, int y, int width, int height, const color_t& color )
{
	x = (x < 0) ? 0 : (x >= m_width) ? m_width : x;
	y = (x < 0) ? 0 : (y >= m_height) ? m_height : y;

	for (int i = 0; i < height; i++) 
	{
		int y1 = y + i;
		if (y1 < 0 || y1 >= m_height) continue;
		for (int rect = 0; rect < width; rect++)
		{
			int x1 = x + rect;
			if (x1 < 0 || x1 >= m_width) continue;
			m_buffer[x1 + (y1 * m_width)] = color;
		}
	}
	
}

void Framebuffer::DrawLineSlope(int x1, int y1, int x2, int y2, const color_t& color)
{
	int dx = x2 - x1;
	int dy = y2 - y1;

	
	if (dx == 0)
	{
		if (y1 > y2) std::swap(y1, y2);
		for (int y = y1; y < y2; y++) 
		{
			m_buffer[x1 + y * m_width] = color;
		}
	}
	else
	{
		float m = dy / (float)dx;
		float b = y1 - (m * x1);

	if (std::abs(dx) > std::abs(dy)) 
	{

	for (int x = x1; x <= x2; x++)
		{

		int y = (int)round(m * x) + b;
		m_buffer[x + y * m_width] = color;

		}
	}
	else {
			
		for (int y = y1; y <= y2; y++)
		{

			int x = (int)round(y-b) / m;
			m_buffer[x + y * m_width] = color;

		}
	}
	}



}

void Framebuffer::DrawLine(int x1, int y1, int x2, int y2, const color_t& color)
{
	int dx = x2 - x1;
	int dy = y2 - y1;

	bool steep = (std::abs(dy) > std::abs(dx));

	if (steep)
	{
		std::swap(x1, y1);
		std::swap(x1, y2);
	}

	if (x1 > x2)
	{
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	dx = x2 - x1;
	dy = std::abs(y2 - y1);

	int error = dx / 2;
	int ystep = (y1 < y2) ? 1 : -1;

	for (int x = x1, y = y1; x <= x2; x++)
	{
		(steep) ? DrawPoint(y, x, color) : DrawPoint(x, y, color);
		error -= dy;
		if (error < 0)
		{
			y += ystep;
			error += dx;
		}
	}

}

void Framebuffer::DrawTrianlge(int x1, int y1, int x2, int y2, int x3, int y3, const color_t& color)
{
	DrawLine(x1, y1, x2, y2, color);
	DrawLine(x2, y2, x3, y3, color);
	DrawLine(x3, y3, x1, y1, color);
}

void Framebuffer::Drawpixel(int xc, int yc, int x, int y, const color_t& color)
{
	DrawPoint(xc + x, yc + y, color);
	DrawPoint(xc - x, yc + y, color);
	DrawPoint(xc + x, yc - y, color);
	DrawPoint(xc - x, yc - y, color);
	DrawPoint(xc + y, yc + x, color);
	DrawPoint(xc - y, yc + x, color);
	DrawPoint(xc + y, yc - x, color);
	DrawPoint(xc - y, yc - x, color);
}

void Framebuffer::DrawCircle(int xc, int yc, int r, const color_t& color)
{
	int x = 0, y = r;
	int d = 3 - 2 * r;
	Drawpixel(xc, yc, x, y, color);
	while (y >= x)
	{
		x++;
		if (d > 0) {
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else {
			d = d + 4 * x + 6;
		}
		
		Drawpixel(xc, yc, x, y, color);
	}
	
}
