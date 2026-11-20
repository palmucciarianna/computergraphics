#version 330 core
in vec2 uv;
uniform float time;
out vec4 fragColor;
void main() {
    // Sfumatura blu-notte, con leggere bande animate.
    vec3 bottom = vec3(0.025, 0.07, 0.15);
    vec3 top = vec3(0.10, 0.22, 0.36);
    vec3 color = mix(bottom, top, uv.y);
    float wave = 0.015 * sin(12.0 * uv.x + 0.7 * time) * sin(9.0 * uv.y);
    fragColor = vec4(color + wave, 1.0);
}
