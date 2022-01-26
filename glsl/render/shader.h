#pragma once
#include "../gl_core_4_3.hpp"
#include "polygon.h"

#include <string>
#include <vector>

class shader
{
public:
    shader();

    void create_fram_shader(const std::string& filePath);
    void craate_vert_shader(const std::string& filePath);
    void create_program();

    void draw_triangle();
    void draw_triangle_alternative();
    void draw_polygons(const std::vector<te::polygon>& polygons);
    void draw_polygon(const te::polygon& pol);

    void draw();

    void get_active_attrib_and_index();
    void get_active_uniform();

private:
    GLuint create_shader(GLenum shaderType, const std::string& filePath);
    GLint print_error(GLenum status, GLuint descr);

    GLuint vertDescr;
    GLuint fragDescr;
    GLuint progDescr;

    GLuint vaoHandle;
};
