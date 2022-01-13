#pragma once
#include "../gl_core_4_3.hpp"

#include <string>

class shader
{
public:
    shader();

    void create_fram_shader(const std::string& filePath);
    void craate_vert_shader(const std::string& filePath);
    void create_program();

    void draw_triangle();
    void draw_triangle_alternative();
    void clear();

    void get_active_attrib_and_index();

private:
    GLuint create_shader(GLenum shaderType, const std::string& filePath);
    GLint print_error(GLenum status, GLuint descr);

    GLuint vertDescr;
    GLuint fragDescr;
    GLuint progDescr;
};
