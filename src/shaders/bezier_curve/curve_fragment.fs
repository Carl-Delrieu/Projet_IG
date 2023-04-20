#version 330 core

in vec3 position;
in vec3 color;

uniform mat4 cameraPosition;

out vec4 FragColor;

void main() {
    FragColor = vec4(1.0f, 1.0f, 1.0f, 0.0f);
}