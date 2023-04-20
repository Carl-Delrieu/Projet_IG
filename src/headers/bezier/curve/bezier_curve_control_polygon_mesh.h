#pragma once

#include "../../../common/defines.h"

class BezierCurveControlPolygonMesh {
private:
    unsigned int m_VBO, m_EBO;

public:
    unsigned int m_VAO;

    std::vector<glm::vec3> m_vertices;

    Camera* m_camera;

    BezierCurveControlPolygonMesh(std::vector<glm::vec3> vertices, Camera* camera)
        : m_vertices(vertices),
        m_camera(camera) {
        setupMesh();
    }

    ~BezierCurveControlPolygonMesh() {
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
        glDrawArrays(GL_LINE_STRIP, 0, m_vertices.size());
        glBindVertexArray(0);
    }
};