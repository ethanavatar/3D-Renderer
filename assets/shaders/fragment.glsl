#version 330 core

uniform float iTime;
uniform ivec2 iResolution;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    gl_FragColor = mix(texture(texture1, TexCoord) * vec4(ourColor, 1.0), texture(texture2, TexCoord), 0.2);
}
