#include <Confuse.h>

#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public Confuse::Layer{
public:
    ExampleLayer() : Layer("example"), m_mainCamera(-1.6f, 1.6f, -0.9f, 0.9f), m_cameraPosition(0.0f){
        m_vertexArray.reset(Confuse::VertexArray::create());

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.1f, 0.1f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.1f, 1.0f, 0.1f, 1.0f,
            0.0f, 0.5f, 0.0f, 0.1f, 0.1f, 1.0f, 1.0f,
        };

        std::shared_ptr<Confuse::VertexBuffer> vertexBuffer;
        vertexBuffer.reset((Confuse::VertexBuffer::create(vertices, sizeof(vertices))));

        Confuse::BufferLayout layout = {
            {Confuse::ShaderDataType::Float3, "a_Position"},
            {Confuse::ShaderDataType::Float4, "a_Color"},
        };

        vertexBuffer->setLayout(layout);
        m_vertexArray->addVertexBuffer(vertexBuffer);

        uint32_t indices[3] = {0, 1, 2};
        std::shared_ptr<Confuse::IndexBuffer> indexBuffer;
        indexBuffer.reset(Confuse::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_vertexArray->setIndexBuffer(indexBuffer);

        m_squareVA.reset(Confuse::VertexArray::create());

        float squareVertices[3 * 4] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f
        };

        std::shared_ptr<Confuse::VertexBuffer> squareVB;
        squareVB.reset((Confuse::VertexBuffer::create(squareVertices, sizeof(squareVertices))));
        squareVB->setLayout({
            {Confuse::ShaderDataType::Float3, "a_Position"}
        });
        m_squareVA->addVertexBuffer(squareVB);

        uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
        std::shared_ptr<Confuse::IndexBuffer> squareIB;
        squareIB.reset(Confuse::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        m_squareVA->setIndexBuffer(squareIB);

        std::string vertexSrc = R"(
            #version 330 core
            
            layout(location = 0) in vec3 a_position;
            layout(location = 1) in vec4 a_color;

            uniform mat4 u_viewProjection;
            uniform mat4 u_transform;

            out vec3 v_position;
            out vec4 v_color;

            void main(){
                v_position = a_position;
                v_color = a_color;
                gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0);
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core
            
            layout(location = 0) out vec4 color;

            in vec3 v_position;
            in vec4 v_color;

            void main(){
                color = vec4(v_position * 0.5 + 0.5, 1.0);
                color = v_color;
            }
        )";

        m_shader.reset(Confuse::Shader::create(vertexSrc, fragmentSrc));

        std::string flatColorVertexSrc = R"(
            #version 330 core
            
            layout(location = 0) in vec3 a_position;

            uniform mat4 u_viewProjection;
            uniform mat4 u_transform;
            
            out vec3 v_position;

            void main(){
                v_position = a_position;
                gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0);
            }
        )";

        std::string flatColorFragmentSrc = R"(
            #version 330 core
            
            layout(location = 0) out vec4 color;

            in vec3 v_position;

            uniform vec3 u_color;

            void main(){
                color = vec4(u_color, 1.0);
            }
        )";
        m_flatColorShader.reset(Confuse::Shader::create(flatColorVertexSrc, flatColorFragmentSrc));
    }

    void onUpdate(Confuse::Timestep ts) override{
        if(Confuse::Input::isKeyPressed(CE_KEY_A))
            m_cameraPosition.x -= m_cameraMoveSpeed * ts;
        else if(Confuse::Input::isKeyPressed(CE_KEY_D))
            m_cameraPosition.x += m_cameraMoveSpeed * ts;
        if(Confuse::Input::isKeyPressed(CE_KEY_W))
            m_cameraPosition.y += m_cameraMoveSpeed * ts;
        else if(Confuse::Input::isKeyPressed(CE_KEY_S))
            m_cameraPosition.y -= m_cameraMoveSpeed * ts;

        if(Confuse::Input::isKeyPressed(CE_KEY_LEFT))
            m_cameraRotation += m_cameraRotationSpeed * ts;
        else if(Confuse::Input::isKeyPressed(CE_KEY_RIGHT))
            m_cameraRotation -= m_cameraRotationSpeed * ts;


        Confuse::RenderCommand::setClearColor({0.06f, 0.06f, 0.06f, 1});
        Confuse::RenderCommand::clear();

        m_mainCamera.setPosition(m_cameraPosition);
        m_mainCamera.setRotation(m_cameraRotation);

        Confuse::Renderer::beginScene(m_mainCamera);

        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

        std::dynamic_pointer_cast<Confuse::OpenGLShader>(m_flatColorShader)->bind();
        std::dynamic_pointer_cast<Confuse::OpenGLShader>(m_flatColorShader)->uploadUniformFloat3("u_color", m_squareColor);

        for(int y = 0; y < 20; y++){
            for(int x = 0; x < 20; x++){
                glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
                Confuse::Renderer::submit(m_flatColorShader, m_squareVA, transform);
            }
        }

        Confuse::Renderer::submit(m_shader, m_vertexArray);

        Confuse::Renderer::endScene();
    }

    virtual void onImGuiRender() override{
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("square color", glm::value_ptr(m_squareColor));
        ImGui::End();
    }

    void onEvent(Confuse::Event& event) override{

    }

private:
    std::shared_ptr<Confuse::Shader> m_shader;
    std::shared_ptr<Confuse::VertexArray> m_vertexArray;

    std::shared_ptr<Confuse::Shader> m_flatColorShader;
    std::shared_ptr<Confuse::VertexArray> m_squareVA;

    Confuse::OrthographicCamera m_mainCamera;
    glm::vec3 m_cameraPosition;
    float m_cameraRotation = 0.0f;

    float m_cameraMoveSpeed = 5.0f;
    float m_cameraRotationSpeed = 180.0f;

    glm::vec3 m_squareColor = {0.2f, 0.3f, 0.8f};
};

class Sandbox : public Confuse::Application{
public:
    Sandbox(){
        pushLayer(new ExampleLayer());
    }

    ~Sandbox(){}
};

Confuse::Application* Confuse::createApplication(){
    return new Sandbox();
}