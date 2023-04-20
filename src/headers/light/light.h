#pragma once

#include "../../common/defines.h"

class Light {
private:
    int m_id;

    glm::vec3 m_position;
    glm::vec3 m_color;

    float m_intensity;

public:
    Light(int id, glm::vec3 position, glm::vec3 color, float intensity) {
        m_id = id;
        m_position = position;
        m_color = color;
        m_intensity = intensity;
    }

    int getId() {
        return m_id;
    }

    glm::vec3 getPosition() {
        return m_position;
    }

    glm::vec3 getColor() {
        return m_color;
    }

    float getIntensity() {
        return m_intensity;
    }
};
