#include "CEpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Confuse{
    OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc){
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        const GLchar *source = vertexSrc.c_str();
        glShaderSource(vertexShader, 1, &source, 0);

        glCompileShader(vertexShader);

        GLint isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(vertexShader);

            CE_CORE_ERROR("{0}", infoLog.data());
            CE_CORE_ASSERT(false, "vertex shader compilation failed!");
            return;
        }

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        source = fragmentSrc.c_str();
        glShaderSource(fragmentShader, 1, &source, 0);

        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(fragmentShader);
            glDeleteShader(vertexShader);
            
            CE_CORE_ERROR("{0}", infoLog.data());
            CE_CORE_ASSERT(false, "fragment shader compilation failed!");
            return;
        }

        m_rendererID = glCreateProgram();
        GLuint program = m_rendererID;

        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

        glLinkProgram(program);

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
            
            glDeleteProgram(program);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
           
            CE_CORE_ERROR("{0}", infoLog.data());
            CE_CORE_ASSERT(false, "shader link failed!");
            return;
        }

        glDetachShader(program, vertexShader);
        glDetachShader(program, fragmentShader);
    }

    OpenGLShader::~OpenGLShader(){
        glDeleteProgram(m_rendererID);
    }

    void OpenGLShader::bind() const{
        glUseProgram(m_rendererID);
    }

    void OpenGLShader::unbind() const{
        glUseProgram(0);
    }

    void OpenGLShader::uploadUniformInt(const std::string& name, int value){
        GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniform1i(location, value);
    }

    void OpenGLShader::uploadUniformFloat(const std::string& name, float value){
        GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniform1f(location, value);
    }
    void OpenGLShader::uploadUniformFloat2(const std::string& name, const glm::vec2& values){
        GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniform2f(location, values.x, values.y);
    }
    void OpenGLShader::uploadUniformFloat3(const std::string& name, const glm::vec3& values){
        GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniform3f(location, values.x, values.y, values.z);
    }
    void OpenGLShader::uploadUniformFloat4(const std::string& name, const glm::vec4& values){
        GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniform4f(location, values.x, values.y, values.z, values.w);
    }

    void OpenGLShader::uploadUniformMat3(const std::string& name, const glm::mat3& matrix){
        GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    void OpenGLShader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix){
        GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

}