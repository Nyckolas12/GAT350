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
    framebuffer.Clear(color_t{255,0,0,255});

    framebuffer.DrawPoint(10, 10, {255,255,255,255});
    for (int i = 0; i < 100; i++)
    {
        int x = rand() % 400;
        int y = rand() % 300;
        framebuffer.DrawPoint(x, y, { 255,255,255,255 });
    }
    
    framebuffer.Update();
    renderer.CopyFramebuffer(framebuffer);
    renderer.BeginFrame();
    
   
   // renderer.EndFrame();
    }
    // clear screen
   
    


    
   // SDL_RenderPresent(renderer.m_renderer);

       
    



    return 0;
}