#include "engine.h"
#include "event.h"
#include "render/shader.h"
#include "render/polygon.h"
#include "render/color.h"

#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include "gl_core_4_3.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl.h>

#include <SDL2/SDL.h>

int main()
{
    std::unique_ptr<te::Engine, void(*)(te::Engine*)> m(te::create_engine(), te::destroy_engine);

    gl::exts::LoadTest didLoad = gl::sys::LoadFunctions();

    if(!didLoad)
    {
        printf("didn't loaded");
    }

    if(!m->initialize())
    {
        return EXIT_FAILURE;
    }

    if(!m->create_window("My window", 0, 0, 300, 300))
    {
        return EXIT_FAILURE;
    }

    printf("%i.\n", didLoad.GetNumMissing());

    std::unique_ptr<shader> new_shader = std::make_unique<shader>();

    float positionData[] =
    {
        -0.8f, -0.8f, 0.0f,
        0.8f, -0.8f, 0.0f,
        0.0f, 0.8f, 0.0f
    };

    float colorData[] =
    {
        1.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 0.f, 1.f
    };

    te::polygon pol(glm::vec3(-0.8f, -0.8f, 0.0f), te::color(1.f, 0.f, 0.f),
                        glm::vec3(0.8f, -0.8f, 0.0f), te::color(0.f, 1.f, 0.f),
                        glm::vec3(0.0f, 0.8f, 0.0f), te::color(0.f, 0.f, 1.f));
    std::vector<te::polygon> polygonsToDraw {pol};

    new_shader->craate_vert_shader("basic.vert");
    new_shader->create_fram_shader("basic.frag");
    new_shader->create_program();
    new_shader->draw_polygon(pol);

    bool continue_loop = true;

    while(continue_loop)
    {
        te::event event;

        while(m->read_input(event))
        {
            switch(event)
            {
            case te::event::up_pressed:
                std::cout << "up" << std::endl;
                break;
            case te::event::turn_off:
                continue_loop = false;
                break;
            }
        }
        new_shader->draw();

        m->swap_buffers();

    };

    new_shader->get_active_attrib_and_index();
    new_shader->get_active_uniform();
    m->unintialize();

    return 0;
}
