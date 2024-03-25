// Vertex shader
#version 420

//uniform mat4 MVP;
uniform mat4 matView;
uniform mat4 matProjection;

// Particle variables
layout (location = 0) in vec4 vPos;
layout (location = 1) in vec2 vUV;
layout (location = 2) in float instanceAlpha;
layout (location = 3) in float instanceLifeTime;
layout (location = 4) in mat4 instanceTransform;

// Fragment shader variables
out vec4 vertexColor;
out vec2 vertexUV;
out float lifetime;

void main()
{
    vertexColor = vec4(1.0) * instanceAlpha;
    vertexUV = vUV;
    lifetime = instanceLifeTime;

    // Billboard particles, always facing the camera
    vec4 viewPos = (matView * instanceTransform[3].xyzw) + (vPos * instanceTransform[0].x);
    gl_Position = matProjection * viewPos;
}

