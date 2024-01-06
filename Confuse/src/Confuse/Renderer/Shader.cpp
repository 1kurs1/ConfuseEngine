#include "CEpch.h"
#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Confuse{
    Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc){
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

    Shader::~Shader(){
        glDeleteProgram(m_rendererID);
    }

    void Shader::bind() const{
        glUseProgram(m_rendererID);
    }

    void Shader::unbind() const{
        glUseProgram(0);
    }

    void Shader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix){
        GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}