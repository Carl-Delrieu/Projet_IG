#pragma once

#include "../../common/defines.h"

class LightMesh {
private:
    unsigned int m_VBO_POS, m_VBO_COL, m_EBO;

    std::vector<glm::vec3> getLightsPosition(std::vector<Light> lights) {
        std::vector<glm::vec3> lightsPosition;
        for (unsigned int i = 0; i < lights.size(); i++) {
            lightsPosition.push_back(lights[i].getPosition());
        }
        return lightsPosition;
    }

    std::vector<glm::vec3> getLightsColor(std::vector<Light> lights) {
        std::vector<glm::vec3> lightsColor;
        for (unsigned int i = 0; i < lights.size(); i++) {
            lightsColor.push_back(lights[i].getColor());
        }
        return lightsColor;
    }

    std::vector<unsigned int> getIndices(std::vector<Light> lights) {
        std::vector<unsigned int> indices;
        for (unsigned int i = 0; i < lights.size(); i++) {
            indices.push_back(i);
        }
        return indices;
    }

    void setupMesh() {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO_POS);
        glGenBuffers(1, &m_VBO_COL);
        glGenBuffers(1, &m_EBO);
        
        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO_POS);
        glBufferData(GL_ARRAY_BUFFER, m_lightsPosition.size() * sizeof(glm::vec3), &m_lightsPosition[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO_COL);
        glBufferData(GL_ARRAY_BUFFER, m_lightsColor.size() * sizeof(glm::vec3), &m_lightsColor[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

        glBindVertexArray(0);
    }


public:
    unsigned int m_VAO;

    std::vector<glm::vec3> m_lightsPosition;
    std::vector<glm::vec3> m_lightsColor;

    std::vector<unsigned int> m_indices;

    Camera* m_camera;

    LightMesh(std::vector<Light> lights, Camera* camera) {
        m_lightsPosition = getLightsPosition(lights);
        m_lightsColor = getLightsColor(lights);
        m_indices = getIndices(lights);
        m_camera = camera;
        setupMesh();
    }

    void draw(Shader* shader, float width, float height) {
        shader->use();

        glm::mat4 projection = glm::perspective(glm::radians(m_camera->Zoom), width / height, 0.1f, 100.0f);
        shader->setMat4("projection", projection);

        glm::vec3 cameraPosition = m_camera->Position;
        shader->setVec3("cameraPosition", cameraPosition);

        glm::mat4 view = m_camera->getViewMatrix();
        shader->setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        shader->setMat4("model", model);

        glPointSize(10.0f);
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_POINTS, 0, m_indices.size());
        glBindVertexArray(0);
    }
};