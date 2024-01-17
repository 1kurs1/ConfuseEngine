#include "Renderer.h"

namespace Utils{
    static uint32_t convertToRGBA(const glm::vec4& color){
        uint8_t r = (uint8_t)(color.r * 255.0f);
        uint8_t g = (uint8_t)(color.g * 255.0f);
        uint8_t b = (uint8_t)(color.b * 255.0f);
        uint8_t a = (uint8_t)(color.a * 255.0f);

        uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
        return result;
    }
}

void Renderer::onResize(uint32_t width, uint32_t height){
    if(m_finalImage){
        if(m_finalImage->getWidth() == width && m_finalImage->getHeight() == height)
            return;
        m_finalImage->resize(width, height);
    }
    else
        m_finalImage = std::make_shared<Confuse::Image>(width, height, Confuse::ImageFormat::RGBA);

    delete[] m_imageData;
    m_imageData = new uint32_t[width * height];
}

void Renderer::render(const Scene& scene, const Camera& camera){
    Ray ray;
	ray.origin = camera.getPosition();

    for(uint32_t y = 0; y < m_finalImage->getHeight(); y++){
        for(uint32_t x = 0; x < m_finalImage->getWidth(); x++){
            ray.direction = camera.getRayDirections()[x + y * m_finalImage->getWidth()];
			glm::vec4 color = traceRay(scene, ray);
            color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
            m_imageData[x + y * m_finalImage->getWidth()] = Utils::convertToRGBA(color);
        }
    }

    m_finalImage->setData(m_imageData);
}

glm::vec4 Renderer::traceRay(const Scene& scene, const Ray& ray){
    // (bx^2 + by^2)t^2 + 2t(axbx + ayby) + (ax^2 + ay^2 - r^2) = 0
    //      a                   b                   c
    // where
    // a = ray origin
    // b = ray direction
    // r = radius
    // t = hit distance

    if(scene.spheres.size() == 0)
        return glm::vec4(0, 0, 0, 1);

    const Sphere* closestSphere = nullptr;
    float hitDistance = std::numeric_limits<float>::max();
    for(const Sphere& sphere: scene.spheres){
        glm::vec3 origin = ray.origin - sphere.position;

        float a = glm::dot(ray.direction, ray.direction);
        float b = 2.0f * glm::dot(origin, ray.direction);
        float c = glm::dot(origin, origin) - sphere.radius * sphere.radius;

        float discriminant = b * b - 4.0f * a * c; 

        if(discriminant < 0.0f)
            continue;
        
        //float t0 = (-b + glm::sqrt(discriminant))/(2.0f*a);
        float closestT = (-b - glm::sqrt(discriminant))/(2.0f*a);
        if(closestT < hitDistance){
            hitDistance = closestT;
            closestSphere = &sphere;
        }
    }

    if(closestSphere == nullptr)
        return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    glm::vec3 origin = ray.origin - closestSphere->position;
    glm::vec3 hitPoint = origin + ray.direction * hitDistance;
    glm::vec3 normal = glm::normalize(hitPoint);

    glm::vec3 lightDir = glm::normalize(glm::vec3(1, -1, -1));
    float lightIntensity = glm::max(glm::dot(normal, -lightDir), 0.0f) * 2;     // == cos(angle)

    glm::vec3 sphereColor = closestSphere->albedo;
    sphereColor *= lightIntensity;
    return glm::vec4(sphereColor, 1.0f);
}