#include "CEpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/gl.h>

namespace Confuse{

    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_windowHandle(windowHandle){
        CE_CORE_ASSERT(m_windowHandle, "window handle is null!");
    }

    void OpenGLContext::init(){
        glfwMakeContextCurrent(m_windowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        CE_CORE_ASSERT(status, "failed to initialize glad!");

        std::cout << "openGL renderer:\n";
        std::cout << "  vendor: " << glGetString(GL_VENDOR) << '\n';
        std::cout << "  renderer: " << glGetString(GL_RENDERER) << '\n';
        std::cout << "  version: " << glGetString(GL_VERSION) << '\n';
    }

    void OpenGLContext::swapBuffers(){
        glBegin(GL_TRIANGLES);
        
        glEnd();

        glfwSwapBuffers(m_windowHandle);
    }
}