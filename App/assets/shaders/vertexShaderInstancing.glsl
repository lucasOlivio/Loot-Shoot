// Vertex shader
#version 420

//uniform mat4 MVP;
uniform mat4 matView;
uniform mat4 matProjection;

// Particle variables
layout (location = 0) in vec4 vPos;
layout (location = 1) in vec2 vUV;
layout (location = 2) in float instanceLifeTime;
layout (location = 3) in mat4 instanceTransform;

// Fragment shader variables
out vec2 vertexUV;
out float lifetime;

void main()
{
    vertexUV = vUV;
    lifetime = instanceLifeTime;

    // Billboard particles, always facing the camera
    float scale = instanceTransform[0].x;
    vec4 position = instanceTransform[3].xyzw;
    vec4 viewPos = (matView * position) + (vPos * scale);

    gl_Position = matProjection * viewPos;
}

