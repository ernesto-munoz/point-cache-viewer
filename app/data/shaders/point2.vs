#version 330

in vec3 vertexPosition;

uniform mat4 mvp;
uniform float uPointSize;

out vec3 vColor;

vec3 hash3(vec3 p) {
    p = vec3(
        dot(p, vec3(127.1, 311.7, 74.7)),
        dot(p, vec3(269.5, 183.3, 246.1)),
        dot(p, vec3(113.5, 271.9, 124.6))
    );
    return fract(sin(p) * 43758.5453123);
}

void main() {
    vColor = 0.3 + 0.7 * hash3(vec3(gl_VertexID)); // color random estable por punto

    gl_Position = mvp * vec4(vertexPosition, 1.0);
    gl_PointSize = uPointSize;
}