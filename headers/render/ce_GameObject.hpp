#pragma once

#include "ce_Model.hpp"

namespace ConfuseEngineRenderer{
    struct TransformComponent{
        glm::vec3 translation{};
        glm::vec3 scale{1.f, 1.f, 1.f};
        glm::vec3 rotation{};

        glm::mat4 mat4();
        glm::mat3 normalMatrix();
    };

    struct PointLightComponent{
        float lightIntensity = 1.0f;
    };

    class CE_GameObject{
    public:
        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, CE_GameObject>;

        static CE_GameObject createGameObject(){
            static id_t currentID = 0;
            return CE_GameObject{currentID++};
        }

        static CE_GameObject makePointLight(float intensity = 10.f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.f));

        CE_GameObject(const CE_GameObject&) = delete;
        CE_GameObject& operator=(const CE_GameObject&) = delete;
        CE_GameObject(CE_GameObject&&) = default;
        CE_GameObject& operator=(CE_GameObject&&) = default;

        id_t getId(){return id;}

        glm::vec3 color{};
        TransformComponent transform{};

        std::shared_ptr<CE_Model> model{};
        std::unique_ptr<PointLightComponent> pointLight = nullptr; 

    private:
        CE_GameObject(id_t objID) : id{objID}{}
        id_t id;
    };
}