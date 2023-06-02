#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aPos2;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// uniform bool pyramid_mode;
uniform float transition;

void main()
{
//    if (pyramid_mode) {
//        gl_Position = projection * view * model * vec4(aPos2, 1.0);
//    } else {
//        gl_Position = projection * view * model * vec4(aPos, 1.0);
//    }
    float alpha = smoothstep(0.0, 1.0, transition);
    vec4 finalPos = vec4(aPos, 1.0) + (vec4(aPos2, 1.0) - vec4(aPos, 1.0)) * alpha;
    gl_Position = projection * view * model * finalPos;
    
    ourColor = aColor;
    ourColor.r = transition;
    // ourColor = vec3(0.0, 1.0, 1.0);
    TexCoord = aTexCoord;
}