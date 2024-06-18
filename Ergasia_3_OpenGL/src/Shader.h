

#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
            // if geometry shader path is present, also load a geometry shader
            if (geometryPath != nullptr)
            {
                gShaderFile.open(geometryPath);
                std::stringstream gShaderStream;
                gShaderStream << gShaderFile.rdbuf();
                gShaderFile.close();
                geometryCode = gShaderStream.str();
            }
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // if geometry shader is given, compile geometry shader
        unsigned int geometry;
        if (geometryPath != nullptr)
        {
            const char* gShaderCode = geometryCode.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, NULL);
            glCompileShader(geometry);
            checkCompileErrors(geometry, "GEOMETRY");
        }
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        if (geometryPath != nullptr)
            glAttachShader(ID, geometry);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if (geometryPath != nullptr)
            glDeleteShader(geometry);

    }
    // activate the shader
    // ------------------------------------------------------------------------
    void use()
    {
        glUseProgram(ID);
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string& name, bool value) const
    {
        int uniformName = glGetUniformLocation(ID, name.c_str());
        if (uniformName == -1) std::cout << "Could not find the " << name << " uniform!!!" << std::endl;
        else glUniform1i(uniformName, (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string& name, int value) const
    {
        int uniformLocation = glGetUniformLocation(ID, name.c_str());
        if (uniformLocation == -1) std::cout << "Could not find the " << name << " uniform!!!" << std::endl;
        else glUniform1i(uniformLocation, value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value) const
    {
        int uniformLocation = glGetUniformLocation(ID, name.c_str());
        if (uniformLocation == -1) std::cout << "Could not find the " << name << " uniform!!!" << std::endl;
        else glUniform1f(uniformLocation, value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string& name, const glm::vec2& value) const
    {
        int uniformLocation = glGetUniformLocation(ID, name.c_str());
        if (uniformLocation == -1) std::cout << "Could not find the " << name << " uniform!!!" << std::endl;
        else glUniform2fv(uniformLocation, 1, &value[0]);
    }
    void setVec2(const std::string& name, float x, float y) const
    {
        int uniformLocation = glGetUniformLocation(ID, name.c_str());
        if (uniformLocation == -1) std::cout << "Could not find the " << name << " uniform!!!" << std::endl;
        else glUniform2f(uniformLocation, x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        int uniformLocation = glGetUniformLocation(ID, name.c_str());
        if (uniformLocation == -1) std::cout << "Could not find the " << name << " uniform!!!" << std::endl;
        else glUniform3fv(uniformLocation, 1, &value[0]);
    }
    void setVec3(const std::string& name, float x, float y, float z) const
    {
        int uniformLocation = glGetUniformLocation(ID, name.c_str());
        if (uniformLocation == -1) std::cout << "Could not find the " << name << " uniform!!!" << std::endl;
        else glUniform3f(uniformLocation, x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string& name, const glm::vec4& value) const
    {
        int uniformLocation = glGetUniformLocation(ID, name.c_str());
        if (uniformLocation == -1) std::cout << "Could not find the " << name << " uniform!!!" << std::endl;
        else glUniform4fv(uniformLocation, 1, &value[0]);
    }
    void setVec4(const std::string& name, float x, float y, float z, float w)
    {
        int uniformLocation = glGetUniformLocation(ID, name.c_str());
        if (uniformLocation == -1) std::cout << "Could not find the " << name << " uniform!!!" << std::endl;
        else glUniform4f(uniformLocation, x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string& name, const glm::mat2& mat) const
    {
        int uniformLocation = glGetUniformLocation(ID, name.c_str());
        if (uniformLocation == -1) std::cout << "Could not find the " << name << " uniform!!!" << std::endl;
        else glUniformMatrix2fv(uniformLocation, 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string& name, const glm::mat3& mat) const
    {
        int uniformLocation = glGetUniformLocation(ID, name.c_str());
        if (uniformLocation == -1) std::cout << "Could not flind the " << name << " uniform!!!" << std::endl;
        else glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        int uniformLocation = glGetUniformLocation(ID, name.c_str());
        if (uniformLocation == -1) std::cout << "Could not find the " << name << " uniform!!!" << std::endl;
        else glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &mat[0][0]);
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};
#endif


