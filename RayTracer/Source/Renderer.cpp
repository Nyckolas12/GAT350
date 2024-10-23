#include "Renderer.h"
#include "Framebuffer.h"
#include <SDL.h>
#include <iostream>

bool Renderer::Initialize()
{
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

bool Renderer::CreateWindow(std::string title, int width, int height)
{
    // create window
    
    m_width = width;
    m_height = height;
    // returns pointer to window if successful or nullptr if failed
     m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (m_window == nullptr)
    {
        std::cerr << "Error creating SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // create renderer
    m_renderer = SDL_CreateRenderer(m_window, -1, 0);

    return 0;
}

void Renderer::CopyFramebuffer(const Framebuffer& framebuffer)
{
    SDL_RenderCopy(m_renderer, framebuffer.m_texture, NULL, NULL);
}

void Renderer::operator=(const Framebuffer& framebuffer)
{
    SDL_RenderCopy(m_renderer, framebuffer.m_texture, NULL, NULL);
}

void Renderer::BeginFrame()
{
    
    // clear screen
    
    SDL_RenderClear(m_renderer);
}

void Renderer::EndFrame()
{
    SDL_RenderPresent(m_renderer);
    
}
