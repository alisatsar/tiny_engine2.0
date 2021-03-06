#include "shader.h"
#include "../file.h"

#include <memory>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

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

    std::string get_type_string(GLenum type)
    {
        if(type == gl::NAME_LENGTH)
        {
            return "NAME_LENGTH";
        }
        else if(type == gl::TYPE)
        {
            return "TYPE";
        }
        else if(type == gl::LOCATION)
        {
            return "LOCATION";
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

void shader::draw()
{
    gl::ClearColor(1.f, 1.0, 0.f, 0.0f);
    gl::Clear(gl::COLOR_BUFFER_BIT);

    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.f), 30.f, glm::vec3(1.f, 0.f, 0.f));

    GLuint location = gl::GetUniformLocation(progDescr, "RotationMatrix");
    if(location >= 0)
    {
        gl::UniformMatrix4fv(location, 1, gl::FALSE_, &rotationMatrix[0][0]);
    }

    gl::BindVertexArray(vaoHandle);
    gl::DrawArrays(gl::TRIANGLES, 0, 3);
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
}


void shader::draw_triangle_alternative()
{
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
}

void shader::draw_polygons(const std::vector<te::polygon>& polygons)
{

}

void shader::draw_polygon(const te::polygon& pol)
{
    float positionData[9];
    float colorData[9];
    for(int i = 0; i < pol.vertexes.size(); ++i)
    {
        positionData[i * pol.vertexes.size() + 0] = pol.vertexes[i].position[0];
        positionData[i * pol.vertexes.size() + 1] = pol.vertexes[i].position[1];
        positionData[i * pol.vertexes.size() + 2] = pol.vertexes[i].position[2];

        colorData[i * pol.vertexes.size() + 0] = pol.vertexes[i].vColor.r;
        colorData[i * pol.vertexes.size() + 1] = pol.vertexes[i].vColor.g;
        colorData[i * pol.vertexes.size() + 2] = pol.vertexes[i].vColor.b;
    }

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
}

void shader::get_active_attrib_and_index()
{
    GLint numAttribs;
    gl::GetProgramInterfaceiv(progDescr, gl::PROGRAM_INPUT, gl::ACTIVE_RESOURCES, &numAttribs);

    GLenum properties[] = { gl::NAME_LENGTH, gl::TYPE, gl::LOCATION };

    printf("Active attributes:\n");
    for(int i = 0; i < numAttribs; ++i)
    {
        GLint results[3];
        gl::GetProgramResourceiv(progDescr, gl::PROGRAM_INPUT, i, 3, properties, 3, NULL, results);

        GLint nameBufSize = results[0] + 1;
        char * name = new char[nameBufSize];
        gl::GetProgramResourceName(progDescr, gl::PROGRAM_INPUT, i, nameBufSize, NULL, name);
        printf("%-5d %s (%s)\n", results[2], name, nShader::get_type_string(results[1]).c_str());
    }
}

void shader::get_active_uniform()
{
    GLint numUniforms = 0;
    gl::GetProgramInterfaceiv(progDescr, gl::UNIFORM, gl::ACTIVE_RESOURCES, &numUniforms);

    GLenum properties[] = {gl::NAME_LENGTH, gl::TYPE, gl::LOCATION, gl::BLOCK_INDEX};

    printf("Active uniforms:\n");

    for(int i = 0; i < numUniforms; ++i)
    {
        GLint results[4];
        gl::GetProgramResourceiv(progDescr, gl::UNIFORM, i, 4, properties, 4, NULL, results);
        if(results[3] != -1)
        {
            continue;
        }
        GLint nameBufSize = results[0] + 1;
        char * name = new char[nameBufSize];
        gl::GetProgramResourceName(progDescr, gl::UNIFORM, i, nameBufSize, NULL, name);
        printf("%-5d %s \n", results[2], name);
        delete [] name;
    }
}
