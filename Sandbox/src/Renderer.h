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
    Renderer() = default;

    void onResize(uint32_t width, uint32_t height);
    void render(const Scene& scene, const Camera& camera);

    std::shared_ptr<Confuse::Image> getFinalImage() const {return m_finalImage;}

private:
    glm::vec4 traceRay(const Scene& scene, const Ray& ray);

private:
    std::shared_ptr<Confuse::Image> m_finalImage;
    uint32_t* m_imageData = nullptr;
};