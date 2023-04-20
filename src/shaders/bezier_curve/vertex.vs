#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 color;
out vec3 position;

void main() {
    color = vec3(1.0f, 1.0f, 1.0f);
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    position = aPos;
}