#pragma once

#include "../../../common/defines.h"


class BezierCurve {
private:
    float m_offset;

    std::vector<glm::vec3> m_controlPoints;
    std::vector<glm::vec3> m_curve;

    std::vector<unsigned int> m_indices;
    
    
    void setupCurve(float n) {
    }

    glm::vec3 deCasteljau(std::vector<glm::vec3> points, float t)  {
        if (points.size() == 1) {
            return points[0];
        } else {
            std::vector<glm::vec3> newPoints;
            for (unsigned int i = 0; i < points.size() - 1; i++) {
                glm::vec3 newPoint = (1 - t) * points[i] + t * points[i + 1];
                newPoints.push_back(newPoint);
            }
            return deCasteljau(newPoints, t);     
        }
    }


    std::vector<glm::vec3> generateCurve(int nbSegments) {
        int nbPoints = nbSegments + 1;
        std::vector<glm::vec3> curvePoints;
        for (int i = 0; i < nbPoints; i++) {
            float t = (float) i * m_offset;
            curvePoints.push_back(deCasteljau(m_controlPoints, t));
        }
        return curvePoints;
    }

    std::vector<unsigned int> generateIndices() {
        std::vector<unsigned int> indices;
        for (int i = 0; i < (int)m_curve.size(); i++) {
            indices.push_back(i);
        }
        return indices;
    }


public:
    BezierCurve(std::vector<glm::vec3> controlPoints, int nbSegments) 
        : m_controlPoints(controlPoints) {
        m_offset = 1.0f / nbSegments;
        m_curve = generateCurve(nbSegments);
        m_indices = generateIndices();
    }
    
    ~BezierCurve() {
        m_controlPoints.clear();
        m_curve.clear();
        m_indices.clear();
    }

    std::vector<glm::vec3> getCurve() {
        return m_curve;
    }

    std::vector<unsigned int> getIndices() {
        return m_indices;
    }

    std::vector<glm::vec3> getControlPolygon() {
        return m_controlPoints;
    }
};