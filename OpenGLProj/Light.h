#pragma once

struct DirLight {

    DirLight() = default;
    DirLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
        : direction(direction), ambient(ambient), diffuse(diffuse), specular(specular) {};

    glm::vec3 direction;
            
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct PointLight {
    PointLight() = default;
    PointLight(const glm::vec3& position, const float& constant, const float& linear, const float& quadratic,
        const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
        : position(position), constant(constant), linear(linear), quadratic(quadratic), ambient(ambient), diffuse(diffuse), specular(specular) {};

    glm::vec3 position;

    float constant;
    float linear;
    float quadratic;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct SpotLight {
    SpotLight() = default;
    SpotLight(const glm::vec3& position,
        const glm::vec3& direction, 
        const float& cutOff,
        const float& outerCutOff,
        const float& constant, 
        const float& linear, 
        const float& quadratic,
        const glm::vec3& ambient, 
        const glm::vec3& diffuse, 
        const glm::vec3& specular)
        : position(position), direction(direction), cutOff(cutOff), outerCutOff(outerCutOff), constant(constant), linear(linear), quadratic(quadratic), ambient(ambient), diffuse(diffuse), specular(specular) {};

    glm::vec3 position;
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};
