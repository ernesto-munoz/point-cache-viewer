#version 330

in vec3 vColor;
out vec4 finalColor;

uniform vec3 uLightDir; // dirección hacia la luz

void main() {
    // uv del sprite, centrado en (0,0), rango [-1,1]
    vec2 uv = gl_PointCoord * 2.0 - 1.0;
    float r2 = dot(uv, uv);
    if (r2 > 1.0) discard; // fuera del círculo -> recorta la forma de esfera

    // normal reconstruida como si fuese una esfera vista de frente
    vec3 normal = normalize(vec3(uv.x, -uv.y, sqrt(1.0 - r2)));

    vec3 L = normalize(uLightDir);
    vec3 V = vec3(0.0, 0.0, 1.0);       // el impostor siempre mira a cámara
    vec3 H = normalize(L + V);

    float ambient  = 0.15;
    float diffuse  = max(dot(normal, L), 0.0);
    float specular = pow(max(dot(normal, H), 0.0), 32.0);

    vec3 color = vColor * (ambient + diffuse) + vec3(1.0) * specular * 0.6;
    finalColor = vec4(color, 1.0);
}