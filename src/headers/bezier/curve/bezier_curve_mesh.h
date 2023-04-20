#pragma once

#include "../../../common/defines.h"

class 
BezierCurveMesh {
private:
    unsigned int m_VBO, m_EBO;

public:
    unsigned int m_VAO;

    std::vector<glm::vec3> m_vertices;
    std::vector<unsigned int> m_indices;
    
    Camera* m_camera;

    
    BezierCurveMesh(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices, Camera* camera) 
        : m_vertices(vertices),
        m_indices(indices),
        m_camera(camera) {
        setupMesh();
    }

    ~BezierCurveMesh() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
    }


    void setupMesh() {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), &m_vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glBindVertexArray(0);
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

        glBindVertexArray(m_VAO);
        glDrawElements(GL_LINE_STRIP, m_indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};