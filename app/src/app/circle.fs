#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform vec2 center;      // centro del círculo en coords de pantalla
uniform float radius;
uniform vec4 innerColor;
uniform vec4 glowColor;
uniform float time;

out vec4 finalColor;

void main()
{
    vec2 fragCoord = gl_FragCoord.xy;
    float dist = distance(fragCoord, center);

    // Borde suave (antialiasing)
    float circle = 1.0 - smoothstep(radius - 2.0, radius, dist);

    // Glow exterior
    float glow = smoothstep(radius + 40.0, radius, dist) * 0.5;

    // Pulso animado opcional
    float pulse = 0.9 + 0.1 * sin(time * 3.0);

    vec4 color = mix(vec4(0.0), innerColor, circle * pulse);
    color += glowColor * glow;

    finalColor = color;
}