#version 330 core

in vec3 color;

uniform vec3 cameraPosition;

out vec4 FragColor;

void main() {
    FragColor = vec4(color, 0.0f);
}
