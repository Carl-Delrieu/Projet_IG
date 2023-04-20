#pragma once

#include "../../../common/defines.h"

class BezierSurface {
private:
    float m_offset;

    std::vector<std::vector<glm::vec3>> m_controlPoints;
    std::vector<std::vector<glm::vec3>> m_surface;

    glm::vec3 m_color;

    
    double factorial(int n) {
        if (n == 0) {
            return 1;
        }
        return n * factorial(n - 1);
    }

    float bernstein(int i, int n, float t) {
        return (float)((double) factorial(n) / (factorial(i) * factorial(n - i)) * pow(t, i) * pow(1 - t, n - i));
    }


    std::vector<std::vector<glm::vec3>> generateSurface(float offset) {
        std::vector<std::vector<glm::vec3>> surface;
        for (float u = 0; u <= 1.0f; u += offset) {
            std::vector<glm::vec3> row;
            for (float v = 0; v <= 1; v += offset) {
                glm::vec3 point = {0.0f, 0.0f, 0.0f};
                for (unsigned int i = 0; i < m_controlPoints.size(); i++) {
                    for (unsigned int j = 0; j < m_controlPoints[i].size(); j++) {
                        float bernsteinI = bernstein(i, m_controlPoints.size() - 1, u);
                        float bernsteinJ = bernstein(j, m_controlPoints[i].size() - 1, v);
                        float bernsteinIJ = bernsteinI * bernsteinJ;
                        point += bernsteinIJ * m_controlPoints[i][j];
                    }
                }
                row.push_back(point);
            }
            surface.push_back(row);
        }
        return surface;
    }

public:
    BezierSurface(std::vector<std::vector<glm::vec3>> controlPoints, glm::vec3 color) 
        : m_controlPoints(controlPoints),
        m_color(color) {
        m_offset = 1.0f / 100;
        m_surface = generateSurface(m_offset);
    }
    
    ~BezierSurface() {
        m_controlPoints.clear();
        m_surface.clear();
    }


    std::vector<std::vector<glm::vec3>> getSurface() {
        return m_surface;
    }

    std::vector<std::vector<glm::vec3>> getControlPolygon() {
        return m_controlPoints;
    }

    glm::vec3 getColor() {
        return m_color;
    }
};