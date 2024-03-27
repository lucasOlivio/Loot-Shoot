// Vertex shader
#version 460 core

struct Particle
{
    vec3 velocity;
    float discard1;
    vec3 acceleration;
    float discard2;
    mat4 transform;
    float lifetime;
    float discard3[3];
};

layout(std430, binding = 0) buffer Particles {
    Particle particles[];
};

uniform mat4 matView;
uniform mat4 matProjection;

// Particle variables
layout (location = 0) in vec4 vPos;
layout (location = 1) in vec2 vUV;

// Fragment shader variables
out vec2 vertexUV;
out float lifetime;

void main()
{
    int index = gl_InstanceID;
    Particle particle = particles[index];

    vertexUV = vUV;
    lifetime = particle.lifetime;

    // Billboard particles, always facing the camera
    float scale = particle.transform[0].x;
    vec4 position = particle.transform[3].xyzw;
    vec4 viewPos = (matView * position) + (vPos * scale);

    gl_Position = matProjection * viewPos;
}

