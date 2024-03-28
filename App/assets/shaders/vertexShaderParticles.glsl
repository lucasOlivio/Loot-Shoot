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
    float initialLifetime;
    float discard3[2];
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
out float initialLifetime;

// Extract the z-axis rotation angle from a 4x4 transform matrix
float ExtractZRotation(in mat4 transform) {
    // Get the elements of the rotation part of the matrix
    float m00 = transform[0][0];
    float m01 = transform[0][1];
    float m10 = transform[1][0];
    float m11 = transform[1][1];

    // Calculate the angle of rotation around the z-axis
    float angle = atan(m01, m00);

    return angle;
}

void main()
{
    int index = gl_InstanceID;
    Particle particle = particles[index];

    vertexUV = vUV;
    lifetime = particle.lifetime;
    initialLifetime = particle.initialLifetime;

    float angle = ExtractZRotation(particle.transform);
    mat4 matrotation = mat4(
        cos(angle), -sin(angle), 0.0, 0.0,
        sin(angle), cos(angle), 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );

    // Billboard particles, always facing the camera
    float scale = particle.transform[0].x;
    vec4 position = particle.transform[3];
    vec4 viewPos = (matView * position) + (vPos * matrotation * scale);

    gl_Position = matProjection * viewPos;
}

