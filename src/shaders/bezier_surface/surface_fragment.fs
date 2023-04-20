#version 330 core

in vec3 position;
in vec3 normal;

uniform vec3 cameraPosition;
uniform vec3 objectColor; 

uniform vec3 lightPositions[3];
uniform vec3 lightColors[3];

out vec4 FragColor;

float ambientStrength = 0.1f;
float specularStrength = 0.5f;

void main() {
    vec3 finalColor = vec3(0.0f);

    vec3 norm = normalize(normal);

    for (int i = 0; i < 3; i++) {
        // Ambient
        vec3 ambient = ambientStrength * lightColors[i];

        // Diffuse
        vec3 lightDir = normalize(lightPositions[i] - position);
        float diff = max(dot(norm, lightDir), 0.0f) * 0.5f;
        vec3 diffuse = diff * lightColors[i];

        // Specular
        vec3 viewDir = normalize(cameraPosition - position);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 64.0f) * 0.8f;
        vec3 specular = specularStrength * spec * lightColors[i];

        finalColor += (ambient + diffuse + specular) * objectColor;
    }

    FragColor = vec4(finalColor, 1.0f);
}