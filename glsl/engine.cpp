#include "engine.h"
#include "event.h"
#include "gl_core_4_3.hpp"

#include <string>
#include <cassert>
#include <algorithm>
#include <array>
#include <iostream>

#include <SDL2/SDL.h>

struct BindedEvent
{
    SDL_Keycode key;
    std::string name;
    te::event pressed;
    te::event released;
};

const std::array<BindedEvent, 8> keys{
    { { SDLK_w, "up", te::event::up_pressed, te::event::up_released },
      { SDLK_a, "left", te::event::left_pressed, te::event::left_released },
      { SDLK_s, "down", te::event::down_pressed, te::event::down_released },
      { SDLK_d, "right", te::event::right_pressed, te::event::right_released },
      { SDLK_LCTRL, "button1", te::event::button_a_pressed,
              te::event::button_a_pressed },
      { SDLK_SPACE, "button2", te::event::button_b_pressed,
              te::event::button_b_pressed },
      { SDLK_ESCAPE, "select", te::event::select_pressed, te::event::select_released },
      { SDLK_RETURN, "start", te::event::start_pressed, te::event::start_released } }
};

bool check_input(const SDL_Event& e, BindedEvent* result)
{
    auto it = std::find_if(std::begin(keys), std::end(keys),
                           [&](const BindedEvent& b)
    {
            return b.key == e.key.keysym.sym;
        } );

    if(it != std::end(keys))
    {
        result->key = it->key;
        result->name = it->name;
        result->pressed = it->pressed;
        result->released = it->released;
        return true;
    }
    return true;
}

class my_tiny_engine
        : public te::Engine
{

public:
    std::string check_version() final
    {
        const GLubyte *renderer = gl::GetString(gl::RENDERER);
        const GLubyte *vendor = gl::GetString(gl::VENDOR);
        const GLubyte *version = gl::GetString(gl::VERSION);
        const GLubyte *glslVersion = gl::GetString(gl::SHADING_LANGUAGE_VERSION);

        GLint major, minor;
        gl::GetIntegerv(gl::MAJOR_VERSION, &major);
        gl::GetIntegerv(gl::MINOR_VERSION, &minor);

        //printf("Vendor : %s\n", vendor);
        //printf("Renderer : %s\n", renderer);
        //printf("Version : %s\n", version);
        //printf("Vendor major %d : minor %d\n", major, minor);
        //printf("GLSL Version : %s\n", glslVersion);
    }

    bool initialize() final
    {
        const int init_result = SDL_Init(SDL_INIT_EVERYTHING);
        if(init_result != 0)
        {
            SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
            return false;
        }
        return true;
    }

    bool create_window(const char* title, int pos_x, int pos_y, int width, int height)
    {
        window = SDL_CreateWindow(
                title,
                pos_x,
                pos_y,
                height,
                width,
                SDL_WINDOW_OPENGL
                );

        if(window == NULL)
        {
            SDL_Log("Could not create window: %s\n", SDL_GetError());
            SDL_Quit();
            return false;
        }


        ///create context for window (for usage function opengl)
        gl_context = SDL_GL_CreateContext(window);
        if(gl_context == NULL)
        {
            std::cout << "GLContext error";
        }
        ///if return 0, then a message is written to the standard error, terminating the program execution
        assert(gl_context != nullptr);

        return true;
    }

    bool read_input(te::event& e)
    {
        SDL_Event sdl_event;

        ///Returns 1 if there is a pending event or 0 if there are none available.
        if(SDL_PollEvent(&sdl_event))
        {
            BindedEvent* binding = nullptr;

            if(sdl_event.type == SDL_QUIT)
            {
                e = te::event::turn_off;
                return true;
            }
            else if(sdl_event.type == SDL_KEYUP)
            {
                if(check_input(sdl_event, binding))
                {
                    e = binding->released;
                    return true;
                }
            }
            else if(sdl_event.type == SDL_KEYDOWN)
            {
                if(check_input(sdl_event, binding))
                {
                    e = binding->pressed;
                    return true;
                }
            }
        }
        return false;
    }

    void swap_buffers() final
    {
        SDL_GL_SwapWindow(window);
    }

    void unintialize() final
    {
        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
private:
    SDL_GLContext gl_context;
    SDL_Window* window = nullptr;

};

te::Engine* te::create_engine()
{
    return new my_tiny_engine();
}

void te::destroy_engine(te::Engine* e)
{
    delete e;
}

te::Engine::~Engine(){}
