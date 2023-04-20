#pragma once

#include "../../../common/defines.h"

class BezierSurfaceMesh {
private:
    unsigned int m_VBO_POS, m_VBO_NOR, m_EBO;


    std::vector<glm::vec3> generateVertices(std::vector<std::vector<glm::vec3>> surface) {
        std::vector<glm::vec3> vertices;
        for (unsigned int i = 0; i < surface.size(); i++) {
            for (unsigned int j = 0; j < surface[i].size(); j++) {
                vertices.push_back(surface[i][j]);
            }
        }
        return vertices;
    }

    glm::vec3 calculateNormal(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3) {
        glm::vec3 vector1 = point2 - point1;
        glm::vec3 vector2 = point3 - point1;
        glm::vec3 normal = glm::cross(vector1, vector2);
        return glm::normalize(normal);
    }

    std::vector<glm::vec3> generateNormals() {
        std::vector<glm::vec3> normals = std::vector<glm::vec3>(m_vertices.size(), glm::vec3(0.0f, 0.0f, 0.0f));
        std::vector<int> nbNormals = std::vector<int>(m_vertices.size(), 0);
        for (unsigned int i = 0; i < m_indices.size(); i += 3) {
            glm::vec3 normal = calculateNormal(m_vertices[m_indices[i]], m_vertices[m_indices[i + 1]], m_vertices[m_indices[i + 2]]);
            normals[m_indices[i]] = normal;
            normals[m_indices[i + 1]] = normal;
            normals[m_indices[i + 2]] = normal;
            for (int j = 0; j < 3; j++) {
                nbNormals[m_indices[i + j]]++;
            }
        }
        for (unsigned int i = 0; i < normals.size(); i++) {
            normals[i] /= nbNormals[i];
            normals[i] = glm::normalize(normals[i]);
        }
        return normals;
    }

    std::vector<unsigned int> generateIndices(std::vector<std::vector<glm::vec3>> surface) {
        std::vector<unsigned int> indices;
        unsigned int i_max = surface.size();
        unsigned int j_max = surface[0].size();
        for (unsigned int i = 0; i < (i_max - 1); i++) {
            for (unsigned int j = 0; j < (j_max - 1); j++) {
                indices.push_back(i * j_max + j + 1);
                indices.push_back(i * j_max + j);
                indices.push_back((i + 1) * j_max + j + 1);

                indices.push_back((i + 1) * j_max + j);
                indices.push_back((i + 1) * j_max + j + 1);
                indices.push_back(i * j_max + j);
            }
        }
        return indices;
    }

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

    void setupMesh() {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO_POS);
        glGenBuffers(1, &m_VBO_NOR);
        glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO_POS);
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), &m_vertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO_NOR);
        glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(glm::vec3), &m_normals[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

public:
    unsigned int m_VAO;

    std::vector<std::vector<glm::vec3>> m_surface;

    glm::vec3 m_color;

    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec3> m_normals;
    
    std::vector<unsigned int> m_indices;

    std::vector<glm::vec3> m_lightPositions;
    std::vector<glm::vec3> m_lightColors;
    
    Camera* m_camera;

    BezierSurfaceMesh(std::vector<std::vector<glm::vec3>> surface, glm::vec3 color, std::vector<Light> lights, Camera* camera)
        : m_surface(surface),
        m_color(color),
        m_camera(camera) {
        m_vertices = generateVertices(surface);
        m_indices = generateIndices(m_surface);
        m_normals = generateNormals();
        m_lightPositions = getLightsPosition(lights);
        m_lightColors = getLightsColor(lights);
        setupMesh();
    }

    ~BezierSurfaceMesh() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO_POS);
        glDeleteBuffers(1, &m_VBO_NOR);
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

        shader->setVec3("objectColor", m_color);

        for (unsigned int i = 0; i < 3; i++) {
            shader->setVec3("lightPositions[" + std::to_string(i) + "]", m_lightPositions[i]);
            shader->setVec3("lightColors[" + std::to_string(i) + "]", m_lightColors[i]);
        }

        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    std::vector<std::vector<glm::vec3>> getSurface() {
        return m_surface;
    }

    std::vector<glm::vec3> getVertices() {
        return m_vertices;
    }

    std::vector<glm::vec3> getNormals() {
        return m_normals;
    }

    std::vector<unsigned int> getIndices() {
        return m_indices;
    }
};