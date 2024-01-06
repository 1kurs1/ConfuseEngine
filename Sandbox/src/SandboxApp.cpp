#include <Confuse.h>

#include "imgui/imgui.h"

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
            -0.75f, -0.75f, 0.0f,
            0.75f, -0.75f, 0.0f,
            0.75f, 0.75f, 0.0f,
            -0.75f, 0.75f, 0.0f
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

            out vec3 v_position;
            out vec4 v_color;

            void main(){
                v_position = a_position;
                v_color = a_color;
                gl_Position = u_viewProjection * vec4(a_position, 1.0);
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

        m_shader.reset(new Confuse::Shader(vertexSrc, fragmentSrc));

        std::string blueShaderVertexSrc = R"(
            #version 330 core
            
            layout(location = 0) in vec3 a_position;

            uniform mat4 u_viewProjection;

            out vec3 v_position;

            void main(){
                v_position = a_position;
                gl_Position = u_viewProjection * vec4(a_position, 1.0);
            }
        )";

        std::string blueShaderFragmentSrc = R"(
            #version 330 core
            
            layout(location = 0) out vec4 color;

            in vec3 v_position;

            void main(){
                color = vec4(0.2, 0.3, 0.8, 1.0);
            }
        )";
        m_blueShader.reset(new Confuse::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
    }

    void onUpdate() override{
        if(Confuse::Input::isKeyPressed(CE_KEY_A))
            m_cameraPosition.x -= m_cameraMoveSpeed;
        else if(Confuse::Input::isKeyPressed(CE_KEY_D))
            m_cameraPosition.x += m_cameraMoveSpeed;
        if(Confuse::Input::isKeyPressed(CE_KEY_W))
            m_cameraPosition.y += m_cameraMoveSpeed;
        else if(Confuse::Input::isKeyPressed(CE_KEY_S))
            m_cameraPosition.y -= m_cameraMoveSpeed;

        if(Confuse::Input::isKeyPressed(CE_KEY_LEFT))
            m_cameraRotation += m_cameraRotationSpeed;
        else if(Confuse::Input::isKeyPressed(CE_KEY_RIGHT))
            m_cameraRotation -= m_cameraRotationSpeed;


        Confuse::RenderCommand::setClearColor({0.06f, 0.06f, 0.06f, 1});
        Confuse::RenderCommand::clear();

        m_mainCamera.setPosition(m_cameraPosition);
        m_mainCamera.setRotation(m_cameraRotation);

        Confuse::Renderer::beginScene(m_mainCamera);
    
        Confuse::Renderer::submit(m_blueShader, m_squareVA);
        Confuse::Renderer::submit(m_shader, m_vertexArray);

        Confuse::Renderer::endScene();
    }

    virtual void onImGuiRender() override{
        
    }

    void onEvent(Confuse::Event& event) override{

    }

private:
    std::shared_ptr<Confuse::Shader> m_shader;
    std::shared_ptr<Confuse::VertexArray> m_vertexArray;

    std::shared_ptr<Confuse::Shader> m_blueShader;
    std::shared_ptr<Confuse::VertexArray> m_squareVA;

    Confuse::OrthographicCamera m_mainCamera;
    glm::vec3 m_cameraPosition;
    float m_cameraRotation = 0.0f;

    float m_cameraMoveSpeed = 0.1f;
    float m_cameraRotationSpeed = 2.0f;
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