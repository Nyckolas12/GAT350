#include "PostProcess.h"
#include "MathUtils.h"
#include <algorithm>
#include <cmath>
void PostProcess::Invert(std::vector<color_t>& buffer)
{
	

	std::for_each(buffer.begin(), buffer.end(), [](auto& color) 
		{
			color.r = 255 - color.r;
			color.g = 255 - color.g;
			color.b = 255 - color.b;
		});
}

void PostProcess::Monochrome(std::vector<color_t>& buffer)
{
	std::for_each(buffer.begin(), buffer.end(), [](auto& color)
		{
			uint8_t average = static_cast<uint8_t>((color.r + color.g + color.b) / 3);

			color.r = average;
			color.g = average;
			color.b = average;
		});
}

void PostProcess::Brightness(std::vector<color_t>& buffer, int brightness)
{
	std::for_each(buffer.begin(), buffer.end(), [brightness](auto& color)
		{

			color.r = static_cast<uint8_t>(Clamp(color.r + brightness, 0, 255));
			color.g = static_cast<uint8_t>(Clamp(color.g + brightness, 0 ,255));
			color.b = static_cast<uint8_t>(Clamp(color.b + brightness, 0 , 255));
		});
}

void PostProcess::ColorBalance(std::vector<color_t>& buffer, int ro, int go, int bo)
{

	
	for (auto& color : buffer)
	{
		color.r = static_cast<uint8_t>(Clamp(color.r + ro, 0, 255));
		color.g = static_cast<uint8_t>(Clamp(color.r + go, 0, 255));
		color.b = static_cast<uint8_t>(Clamp(color.r + bo, 0, 255));
	}
}

void PostProcess::Noise(std::vector<color_t>& buffer, uint8_t noise)
{

	for(auto& color : buffer)
		{
			int r_offset = (rand() % ((noise * 2) + 1)) - noise;
			int g_offset = (rand() % ((noise * 2) + 1)) - noise;
			int b_offset = (rand() % ((noise * 2) + 1)) - noise;
			color.r = static_cast<uint8_t>(Clamp(color.r + r_offset, 0, 255));
			color.g = static_cast<uint8_t>(Clamp(color.g + g_offset, 0, 255));
			color.b = static_cast<uint8_t>(Clamp(color.b + b_offset, 0, 255));
		}
}

void PostProcess::Threshold(std::vector<color_t>& buffer, uint8_t threshold)
{
	for (auto& color : buffer)
	{
		uint8_t luminance = static_cast<uint8_t>(0.299 * color.r + 0.587 * color.g + 0.114 * color.b);
		if (luminance >= threshold)
		{
			color.r = 255;
			color.g = 255;
			color.b = 255;
		}
		else
		{
			color.r = 0;
			color.g = 0;
			color.b = 0;
		}
	}

}

void PostProcess::Posterize(std::vector<color_t>& buffer, uint8_t levels)
{
	uint8_t level = 255 / levels;
	std::for_each(buffer.begin(), buffer.end(), [level](color_t& color)
		{
			color.r = (color.r / level) * level;
			color.g = (color.g / level) * level;
			color.b = (color.b / level) * level;
		});
}

void PostProcess::BoxBlur(std::vector<color_t>& buffer, int width, int height)
{
	std::vector<color_t> source = buffer;

	int k[3][3] =
	{
		{ 1,1,1 },
		{ 1,1,1 },
		{ 1,1,1 }
	};

	for (int i = 0; i < buffer.size(); i++)
	{
		int x = i % width; 
		int y = i / width; 

		if (x < 1 || x + 1 >= width || y < 1 || y + 1 >= height) continue;

		int r = 0;
		int b = 0;
		int g = 0;

		for (int iy = 0; iy < 3; iy++)
		{
			for (int ix = 0; ix < 3; ix++)
			{
				color_t pixel = source[(x + ix - 1)+ (y + iy - 1) * width];
				int weight = k[iy][ix];

				r += pixel.r * weight;
				b += pixel.b * weight;
				g += pixel.g * weight;
			}
		}
	color_t& color = buffer[i];
	color.r = static_cast<uint8_t>(r / 9);
	color.b = static_cast<uint8_t>(b / 9);
	color.g = static_cast<uint8_t>(g / 9);
	}
}

void PostProcess::GaussianBlur(std::vector<color_t>& buffer, int width, int height)
{
	std::vector<color_t> source = buffer;

	int k[3][3] =
	{
		{ 1,2,1 },
		{ 2,4,2 },
		{ 1,2,1 }
	};

	for (int i = 0; i < buffer.size(); i++)
	{
		int x = i % width;
		int y = i / width;

		if (x < 1 || x + 1 >= width || y < 1 || y + 1 >= height) continue;

		int r = 0;
		int b = 0;
		int g = 0;

		for (int iy = 0; iy < 3; iy++)
		{
			for (int ix = 0; ix < 3; ix++)
			{
				color_t pixel = source[(x + ix - 1) + (y + iy - 1) * width];
				int weight = k[iy][ix];

				r += pixel.r * weight;
				b += pixel.b * weight;
				g += pixel.g * weight;
			}
		}
		color_t& color = buffer[i];
		color.r = static_cast<uint8_t>(r / 16);
		color.b = static_cast<uint8_t>(b / 16);
		color.g = static_cast<uint8_t>(g / 16);
	}
}

void PostProcess::Sharpen(std::vector<color_t>& buffer, int width, int height)
{
	std::vector<color_t> source = buffer;

	int k[3][3] =
	{
		{ 0,-1,0 },
		{ -1,5,-1 },
		{ 0,-1,0 }
	};

	for (int i = 0; i < buffer.size(); i++)
	{
		int x = i % width;
		int y = i / width;

		if (x < 1 || x + 1 >= width || y < 1 || y + 1 >= height) continue;

		int r = 0;
		int b = 0;
		int g = 0;

		for (int iy = 0; iy < 3; iy++)
		{
			for (int ix = 0; ix < 3; ix++)
			{
				color_t pixel = source[(x + ix - 1) + (y + iy - 1) * width];
				int weight = k[iy][ix];

				r += pixel.r * weight;
				b += pixel.b * weight;
				g += pixel.g * weight;
			}
		}
		color_t& color = buffer[i];
		color.r = static_cast<uint8_t>(Clamp(r,0,255));
		color.b = static_cast<uint8_t>(Clamp(b, 0, 255));
		color.g = static_cast<uint8_t>(Clamp(g, 0, 255));
	}
}

void PostProcess::Edge(std::vector<color_t>& buffer, int width, int height, int threshold)
{
	std::vector<color_t> source = buffer;

	int hk[3][3] =
	{
		{ 1,0,-1 },
		{ 2,0,-2 },
		{ 1,0,-1 }
	};
	int vk[3][3] =
	{
		{ 1,2,1 },
		{ 0,0,0 },
		{ -1,-2,-1 }
	};

	for (int i = 0; i < buffer.size(); i++)
	{
		int x = i % width;
		int y = i / width;

		if (x < 1 || x + 1 >= width || y < 1 || y + 1 >= height) continue;

		int h = 0;
		int v = 0;

		for (int iy = 0; iy < 3; iy++)
		{
			for (int ix = 0; ix < 3; ix++)
			{
				color_t pixel = source[(x + ix - 1) + (y + iy - 1) * width];
				h += pixel.r * hk[iy][ix];
				v += pixel.r * vk[iy][ix];
			}
		}
		int m = static_cast<int>(std::sqrt(h * h + v * v));
		m = (m >= threshold) ? m : 0;
		uint8_t c = std::clamp(m, 0, 255);
		color_t& color = buffer[i];
		color.r = c;
		color.b = c;
		color.g = c;
	}
}

void PostProcess::Emboss(std::vector<color_t>& buffer, int width, int height)
{
	std::vector<color_t> source = buffer;

	int k[3][3] =
	{
		{-1, -1,  0},
		{ -1,  0,  1 },
		{ 0,  1,  1 }
	};

	for (int i = 0; i < buffer.size(); i++)
	{
		int x = i % width;
		int y = i / width;

		if (x < 1 || x  >= width - 1 || y < 1 || y  >= height - 1) continue;

		int r = 128;
		int b = 128;
		int g = 128;

		for (int iy = 0; iy < 3; iy++)
		{
			for (int ix = 0; ix < 3; ix++)
			{
				color_t pixel = source[(x + ix - 1) + (y + iy - 1) * width];
				

				r += pixel.r * k[iy][ix];
				b += pixel.b * k[iy][ix];
				g += pixel.g * k[iy][ix ];
			}
		}
		color_t& color = buffer[i];
		color.r = static_cast<uint8_t>(Clamp(r, 0, 255));
		color.b = static_cast<uint8_t>(Clamp(b, 0, 255));
		color.g = static_cast<uint8_t>(Clamp(g, 0, 255));
	}
}