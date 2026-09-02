#version 330

in vec3 vertexPosition;

uniform mat4 mvp;
uniform float uPointSize;

void main() {
    gl_Position = mvp * vec4(vertexPosition, 1.0);
    gl_PointSize = uPointSize;
}