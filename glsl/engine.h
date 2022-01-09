#pragma once
#include <string>

namespace te
{

enum class event;

class Engine
{
public:
    virtual ~Engine();
    virtual std::string check_version() = 0;
    virtual bool initialize() = 0;
    virtual bool create_window(
            const char* title, int pos_x, int pos_y, int width, int height) = 0;
    virtual bool read_input(event& e) = 0;
//virtual void clear_color(const triangle& t) = 0;
    virtual void unintialize() = 0;
    virtual void swap_buffers() = 0;
};

Engine* create_engine();
void destroy_engine(Engine* e);
}
