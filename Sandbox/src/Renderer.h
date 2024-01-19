#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "Confuse/Image.h"
#include "Confuse/Random.h"

#include "Camera.h"
#include "Ray.h"
#include "Scene.h"

class Renderer{
public:
    struct Settings{
        bool accumulate = true;
    };

public:
    Renderer() = default;

    void onResize(uint32_t width, uint32_t height);
    void render(const Scene& scene, const Camera& camera);

    std::shared_ptr<Confuse::Image> getFinalImage() const {return m_finalImage;}

    void resetFrameIndex() {m_frameIndex = 1;}
    Settings& getSettings() {return m_settings;}

private:
    struct HitPayload{
        float hitDistance;
        glm::vec3 worldPosition;
        glm::vec3 worldNormal;

        int objectIndex; 
    };

    glm::vec4 perPixel(uint32_t x, uint32_t y);
    HitPayload traceRay(const Ray& ray);
    HitPayload closestHit(const Ray& ray, float hitDistance, int objectIndex);
    HitPayload miss(const Ray& ray);

private:
    std::shared_ptr<Confuse::Image> m_finalImage;
    Settings m_settings;

    const Scene* m_activeScene = nullptr;
    const Camera* m_activeCamera = nullptr;

    uint32_t* m_imageData = nullptr;
    glm::vec4* m_accumulationData = nullptr;

    uint32_t m_frameIndex = 1;
};