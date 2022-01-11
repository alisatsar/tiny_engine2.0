#include "shader.h"
#include "../file.h"

#include <memory>
#include <iostream>

namespace nShader {
    const std::string ShaderDirectory{"/shaders/"};

    std::string GetErrorLog(GLenum type)
    {
        if(type == gl::COMPILE_STATUS)
        {
            return "Shader compilation failed\n";
        }
        else if(type == gl::LINK_STATUS)
        {
            return "Link program failed\n";
        }
        else
        {
            return "";
        }
    }
}

shader::shader()
{

}

void shader::craate_vert_shader(const std::string& filePath)
{
    vertDescr = create_shader(gl::VERTEX_SHADER, nShader::ShaderDirectory + filePath);
    GLint result;
    GLenum status = gl::COMPILE_STATUS;
    gl::GetShaderiv(vertDescr, status, &result);
    if(gl::FALSE_ == result)
    {
        fprintf(stderr, "error vertex shader\n");
        print_error(status, vertDescr);
    }
}

void shader::create_fram_shader(const std::string& filePath)
{
    fragDescr = create_shader(gl::FRAGMENT_SHADER, nShader::ShaderDirectory + filePath);
    GLint result;
    GLenum status = gl::COMPILE_STATUS;
    gl::GetShaderiv(fragDescr, status, &result);
    if(gl::FALSE_ == result)
    {
        fprintf(stderr, "error fragment shader\n");
        print_error(status, fragDescr);
    }
}

void shader::create_program()
{
    progDescr = gl::CreateProgram();
    if(0 == progDescr)
    {
        fprintf(stderr, "Error creating program object.\n");
        exit(1);
    }

    gl::AttachShader(progDescr, vertDescr);
    gl::AttachShader(progDescr, fragDescr);

    gl::LinkProgram(progDescr);

    GLint result;
    GLenum status = gl::LINK_STATUS;
    gl::GetProgramiv(progDescr, status, &result);
    if(gl::FALSE_ == status)
    {
        print_error(status, progDescr);
    }
    else
    {
        gl::UseProgram(progDescr);
    }
}

void shader::clear()
{
    gl::ClearColor(1.f, 1.0, 0.f, 0.0f);
    gl::Clear(gl::COLOR_BUFFER_BIT);
}

GLuint shader::create_shader(GLenum shaderType, const std::string& filePath)
{
    GLuint shaderDescr = gl::CreateShader(shaderType);
    if(0 == shaderDescr)
    {
        fprintf(stderr, "Error creating shader.\n");
        exit(EXIT_FAILURE);
    }

    auto shaderFile = std::make_unique<te::file>(filePath);
    shaderFile->read();
    std::string shaderFileContext = shaderFile->get_context();
    const GLchar * shaderCode = shaderFileContext.c_str();
    std::cout << shaderCode << std::endl;

    const GLchar * codeArray[] = {shaderCode};
    gl::ShaderSource (shaderDescr, 1, codeArray, NULL);
    gl::CompileShader(shaderDescr);

    return shaderDescr;
}

GLint shader::print_error(GLenum status, GLuint descr)
{
    fprintf(stderr, nShader::GetErrorLog(status).c_str());

    GLint logLen;
    gl::GetShaderiv(descr, gl::INFO_LOG_LENGTH, &logLen);
    if(logLen > 0)
    {
        char * log = new char[logLen];

        GLsizei written;
        gl::GetShaderInfoLog(descr, logLen, &written, log);

        fprintf(stderr, "Shader log: \n%s", log);
        delete [] log;
    }
}

void shader::draw_triangle()
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

void shader::draw_triangle_alternative()
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

    gl::BindVertexBuffer(0, positionBufferDescr, 0, sizeof(GLfloat)*3);
    gl::BindVertexBuffer(1, colorBufferDescr, 0, sizeof(GLfloat)*3);

    gl::VertexAttribFormat(0, 3, gl::FLOAT, gl::FALSE_, 0);
    gl::VertexAttribBinding(0, 0);
    gl::VertexAttribFormat(1, 3, gl::FLOAT, gl::FALSE_, 0);
    gl::VertexAttribBinding(1, 1);


    gl::BindVertexArray(vaoHandle);
    gl::DrawArrays(gl::TRIANGLES, 0, 3);
}
