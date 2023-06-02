#version 330 core

out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

uniform bool pyramid_mode;
uniform float transition;

void main() {
    FragColor = vec4(ourColor, 1.0);
    // FragColor = texture(ourTexture, TexCoord);
}