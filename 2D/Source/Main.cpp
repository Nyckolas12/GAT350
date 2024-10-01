#include <SDL.h>
#include <iostream>
#include "Renderer.h"

int main(int argc, char* argv[])
{
    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>();
    renderer->Initialize();
    renderer->CreateWindow();

    while (true)
    {
        renderer->BeginFrame();
        renderer->EndFrame();



    }



    return 0;
}