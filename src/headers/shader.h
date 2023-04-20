#pragma once

#include "../common/defines.h"

class Shader {
private:
    void errorCheck(unsigned int shader, std::string type) {
        int success;
        char infoLog[1024];

        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << std::endl;
            }
        }
    }

public:
    unsigned int m_ID;

    Shader(const std::string& vertexPath, const std::string& fragmentPath) {
        std::string vertexCode, fragmentCode;
        std::ifstream vertexShaderFile, fragmentShaderFile;

        vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            vertexShaderFile.open(vertexPath);

            std::stringstream vertexShaderStream;
            vertexShaderStream << vertexShaderFile.rdbuf();
            
            vertexShaderFile.close();

            vertexCode = vertexShaderStream.str();

        } catch (std::ifstream::failure e) {
            std::cout << "ERROR::SHADER::VERTEX::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
        }

        try {
            fragmentShaderFile.open(fragmentPath);

            std::stringstream fragmentShaderStream;
            fragmentShaderStream << fragmentShaderFile.rdbuf();

            fragmentShaderFile.close();

            fragmentCode = fragmentShaderStream.str();

        } catch (std::ifstream::failure e) {
            std::cout << "ERROR::SHADER::FRAGMENT::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
        }

        const char* vertexShaderCode = vertexCode.c_str();
        const char* fragmentShaderCode = fragmentCode.c_str();

        unsigned int vertexShader, fragmentShader;
        
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
        glCompileShader(vertexShader);
        errorCheck(vertexShader, "VERTEX");

        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
        glCompileShader(fragmentShader);
        errorCheck(fragmentShader, "FRAGMENT");

        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertexShader);
        glAttachShader(m_ID, fragmentShader);
        glLinkProgram(m_ID);
        errorCheck(m_ID, "PROGRAM");

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    ~Shader() {
        glDeleteProgram(m_ID);
    }


    void use() {
        glUseProgram(m_ID);
    }
    
    unsigned int getID() {
        return m_ID;
    }
    

    void setBool(const std::string& name, bool value) {
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
    }
    
    void setInt(const std::string& name, int value) {
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
    }
    
    void setFloat(const std::string& name, float value) {
        glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
    }
    
    
    void setVec2(const std::string& name, const glm::vec2& vec) {
        glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, &vec[0]);
    }
    
    void setVec2(const std::string& name, float x, float y) {
        glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
    }
    
    
    void setVec3(const std::string& name, const glm::vec3& vec) {
        glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &vec[0]);
    }
    
    void setVec3(const std::string& name, float x, float y, float z) {
        glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
    }
    
    
    void setVec4(const std::string& name, const glm::vec4& vec) {
        glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, &vec[0]);
    }
    
    void setVec4(const std::string& name, float x, float y, float z, float w) {
        glUniform4f(glGetUniformLocation(m_ID, name.c_str()), x, y, z, w);
    }
    
    
    void setMat2(const std::string& name, const glm::mat2& mat) {
        glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    
    void setMat3(const std::string& name, const glm::mat3& mat) {
        glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    
    void setMat4(const std::string& name, const glm::mat4& mat) {
        glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
};