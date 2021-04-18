#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in float aState;

out float State;

void main()
{
    gl_Position = vec4(aPos, 1.0f);
    State = aState;
}

