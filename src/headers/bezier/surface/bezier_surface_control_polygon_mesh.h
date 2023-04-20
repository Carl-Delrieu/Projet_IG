#pragma once

#include "../../../common/defines.h"

class BezierSurfaceControlPolygonMesh {
private:
    unsigned int m_VBO, m_EBO;


    std::vector<glm::vec3> generateVertices(std::vector<std::vector<glm::vec3>> controlPoints) {
        std::vector<glm::vec3> vertices;
        for (unsigned int i = 0; i < controlPoints.size(); i++) {
            for (unsigned int j = 0; j < controlPoints[i].size(); j++) {
                vertices.push_back(controlPoints[i][j]);
            }
        }
        return vertices;
    }

    std::vector<unsigned int> generateIndices(std::vector<std::vector<glm::vec3>> controlPoints) {
        std::vector<unsigned int> indices;
        unsigned int i_max = controlPoints.size();
        unsigned int j_max = controlPoints[0].size();
        for (unsigned int i = 0; i < (i_max - 1); i++) {
            for (unsigned int j = 0; j < (j_max - 1); j++) {
                indices.push_back(i * j_max + j);
                indices.push_back(i * j_max + j + 1);
                indices.push_back(i * j_max + j);
                indices.push_back((i + 1) * j_max + j);
            }
            indices.push_back(i * j_max + j_max - 1);
            indices.push_back((i + 1) * j_max + j_max - 1);
        }
        for (unsigned int j = 0; j < controlPoints[i_max - 1].size() - 1; j++) {
            indices.push_back((i_max - 1) * j_max + j);
            indices.push_back((i_max - 1) * j_max + j + 1);
        }
        return indices;
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

public:
    unsigned int m_VAO;

    std::vector<std::vector<glm::vec3>> m_controlPoints;
    std::vector<glm::vec3> m_vertices;
    std::vector<unsigned int> m_indices;

    Camera* m_camera;

    BezierSurfaceControlPolygonMesh(std::vector<std::vector<glm::vec3>> controlPoints, Camera* camera)
        : m_controlPoints(controlPoints),
        m_camera(camera) {
        m_vertices = generateVertices(m_controlPoints);
        m_indices = generateIndices(m_controlPoints);
        setupMesh();    
    }

    ~BezierSurfaceControlPolygonMesh() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
    }

    void draw(Shader* shader, float width, float height) {
        shader->use();

        glm::mat4 projection = glm::perspective(glm::radians(m_camera->Zoom), width / height, 0.1f, 100.0f);
        shader->setMat4("projection", projection);

        glm::vec3 cameraPositon = m_camera->Position;
        shader->setVec3("cameraPosition", cameraPositon);

        glm::mat4 view = m_camera->getViewMatrix();
        shader->setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        shader->setMat4("model", model);


        glBindVertexArray(m_VAO);
        glDrawElements(GL_LINES, m_indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};