#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include "gl_core_4_3.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl.h>

#include <SDL2/SDL.h>

#include "engine.h"
#include "event.h"

void print_versions();
GLuint create_shader(GLenum shaderType, const std::string& filePath);
void load_shader_as_string(const std::string& file_path, std::string& vertex_content);
GLuint create_program(GLuint verShaderDescr, GLuint fragShaderDescr);
void draw_triangle(GLuint program_handle);

int main()
{
    std::unique_ptr<te::Engine, void(*)(te::Engine*)> m(te::create_engine(), te::destroy_engine);

    std::cout << m->check_version();

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
    print_versions();
    std::string vertexShaderFilePath = "/home/alisa/Documents/glsl_project/glsl/shaders/basic.vert";
    GLuint vertexShaderDescr = create_shader(gl::VERTEX_SHADER, vertexShaderFilePath);

    std::string fragShaderFilePath = "/home/alisa/Documents/glsl_project/glsl/shaders/basic.frag";
    GLuint fragShaderDescr = create_shader(gl::FRAGMENT_SHADER, fragShaderFilePath);
    GLuint progDescr = create_program(vertexShaderDescr, fragShaderDescr);

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

        draw_triangle(progDescr);
        m->swap_buffers();
        gl::ClearColor(1.f, 1.0, 0.f, 0.0f);
        gl::Clear(gl::COLOR_BUFFER_BIT);
    };

    m->unintialize();


    return 0;
}

void draw_triangle(GLuint program_handle)
{
    GLuint vaoHandle;

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

    GLuint vboHandles[2];
    gl::GenBuffers(2, vboHandles);
    GLuint positionBufferDescr = vboHandles[0];
    GLuint colorBufferDescr = vboHandles[1];

    gl::BindBuffer(gl::ARRAY_BUFFER, positionBufferDescr);
    gl::BufferData(gl::ARRAY_BUFFER,
                   9 * sizeof(float),
                   positionData,
                   gl::STATIC_DRAW);

    gl::BindBuffer(gl::ARRAY_BUFFER, colorBufferDescr);
    gl::BufferData(gl::ARRAY_BUFFER,
                   9 * sizeof(float),
                   colorData,
                   gl::STATIC_DRAW);

    gl::GenVertexArrays(1, &vaoHandle);
    gl::BindVertexArray(vaoHandle);

    gl::EnableVertexAttribArray(0);
    gl::EnableVertexAttribArray(1);

    gl::BindBuffer(gl::ARRAY_BUFFER, positionBufferDescr);
    gl::VertexAttribPointer(0, 3, gl::FLOAT, gl::FALSE_, 0, NULL);

    gl::BindBuffer(gl::ARRAY_BUFFER, colorBufferDescr);
    gl::VertexAttribPointer(1, 3, gl::FLOAT, gl::FALSE_, 0, NULL);

    gl::BindVertexArray(vaoHandle);
    gl::DrawArrays(gl::TRIANGLES, 0, 3);

}

void print_versions()
{
    const GLubyte *renderer = gl::GetString(gl::RENDERER);
    const GLubyte *vendor = gl::GetString(gl::VENDOR);
    const GLubyte *version = gl::GetString(gl::VERSION);
    const GLubyte *glslVersion = gl::GetString(gl::SHADING_LANGUAGE_VERSION);

    GLint major, minor;
    gl::GetIntegerv(gl::MAJOR_VERSION, &major);
    gl::GetIntegerv(gl::MINOR_VERSION, &minor);

    printf("Vendor : %s\n", vendor);
    printf("Renderer : %s\n", renderer);
    printf("Version : %s\n", version);
    printf("Vendor major %d : minor %d\n", major, minor);
    printf("GLSL Version : %s\n", glslVersion);

}

GLuint create_program(GLuint verShaderDescr, GLuint fragShaderDescr)
{
    GLuint programHandle = gl::CreateProgram();
    if(0 == programHandle)
    {
        fprintf(stderr, "Error creating program object.\n");
        exit(1);
    }

    gl::AttachShader(programHandle, verShaderDescr);
    gl::AttachShader(programHandle, fragShaderDescr);

    gl::LinkProgram(programHandle);

    GLint status;
    gl::GetProgramiv(programHandle, gl::LINK_STATUS, &status);
    if(gl::FALSE_ == status)
    {
        fprintf(stderr, "Failed to link shader program!\n");
        GLint logLen;

        gl::GetProgramiv(programHandle, gl::INFO_LOG_LENGTH, &logLen);

        if(logLen > 0)
        {
            char* log = new char[logLen];

            GLsizei written;

            gl::GetProgramInfoLog(programHandle, logLen, &written, log);
            fprintf(stderr, "Program log: \n%s", log);
            delete [] log;
        }
    }
    else
    {
        gl::UseProgram(programHandle);
    }
    return programHandle;
}

GLuint create_shader(GLenum shaderType, const std::string& filePath)
{
    GLuint vertShader = gl::CreateShader(shaderType);
    if(0 == vertShader)
    {
        fprintf(stderr, "Error creating vertex shader.\n");
        exit(EXIT_FAILURE);
    }

    std::string vertexContent;
    load_shader_as_string(filePath, vertexContent);
    const GLchar * shaderCode = vertexContent.c_str();
    std::cout << filePath << std::endl;

    const GLchar * codeArray[] = {shaderCode};
    gl::ShaderSource (vertShader, 1, codeArray, NULL);
    gl::CompileShader(vertShader);
    GLint result;
    gl::GetShaderiv(vertShader, gl::COMPILE_STATUS, &result);
    if(gl::FALSE_ == result)
    {
        fprintf(stderr, "Vertex shader compilation failed\n");

        GLint logLen;
        gl::GetShaderiv(vertShader, gl::INFO_LOG_LENGTH, &logLen);
        if(logLen > 0)
        {
            char * log = new char[logLen];

            GLsizei written;
            gl::GetShaderInfoLog(vertShader, logLen, &written, log);

            fprintf(stderr, "Shader log: \n%s", log);
            delete [] log;
        }
    }
    return vertShader;
}

void load_shader_as_string(const std::string& file_path, std::string& vertex_content)
{
    std::ifstream vertex_file(file_path);
    if(vertex_file.fail())
    {
        perror(file_path.c_str());
    }

    std::string line;

    while(std::getline(vertex_file, line))
    {
        vertex_content += line + "\n";

    }
    std::cout << vertex_content << std::endl;
    vertex_file.close();
}
