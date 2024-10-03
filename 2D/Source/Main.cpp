#include <SDL.h>
#include <iostream>
#include "Renderer.h"
#include "Framebuffer.h"

int main(int argc, char* argv[])
{
  
    Renderer renderer;
   
    renderer.Initialize();
    renderer.CreateWindow("Game Engine", 800, 600);
   Framebuffer framebuffer(renderer, 800, 600);
    bool quit = false;

    SDL_Event event;
    while(SDL_PollEvent(&event))
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

    while (true) 
    {
    framebuffer.Clear(color_t{0,0,0,0});
    renderer.BeginFrame();
    //framebuffer.DrawPoint(10, 10, {255,255,255,255});
    for (int i = 0; i < 300; i++)
     {
        int x = rand() % framebuffer.m_width;
        int y = rand() % framebuffer.m_height;
        int x2 = rand() % framebuffer.m_width;
        int x3 = rand() % framebuffer.m_width;
        int y2 = rand() % framebuffer.m_height;
        int y3 = rand() % framebuffer.m_height;
        framebuffer.DrawPoint(x, y, { 55,100,156,255 });
       // framebuffer.DrawLine(x, y, x2, y2, { 255,45,67 });
        //framebuffer.DrawRect(x, y, 40, 40, { 255,255,255 });
        //framebuffer.DrawTrianlge(x, y, x2, y2, x3, y3, { 255,56,76,255 });
        }
    framebuffer.Update();
    renderer.CopyFramebuffer(framebuffer);

    renderer.EndFrame();

    }

    

    return 0;
}