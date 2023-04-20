#pragma once

#include "../../../common/defines.h"

class BezierSurfaceNormalMesh {
private:
    unsigned int m_VBO, m_EBO;


    std::vector<glm::vec3> generateVertices(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals) {
        std::vector<glm::vec3> verticesNormals;
        for (unsigned int i = 0; i < vertices.size(); i++) {
            verticesNormals.push_back(vertices[i]);
            verticesNormals.push_back(vertices[i] + normals[i] * 0.1f);
        }
        return verticesNormals;
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
    }

public:
    unsigned int m_VAO;

    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec3> m_normals;

    Camera* m_camera;

    BezierSurfaceNormalMesh(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, Camera* camera) {
        m_vertices = generateVertices(vertices, normals);
        m_normals = normals;
        m_camera = camera;
        setupMesh();
    }

    ~BezierSurfaceNormalMesh() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
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
        glDrawArrays(GL_LINES, 0, m_vertices.size());
        glBindVertexArray(0);
    }
};