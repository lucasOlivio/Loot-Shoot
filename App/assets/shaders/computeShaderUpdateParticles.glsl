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

layout(std430, binding = 1) buffer FreeList {
    int count;
    int indices[];
} freelist;

layout(location = 0) uniform float deltaTime;

void UpdateParticle(inout Particle particle, int index) {
    if (particle.lifetime <= 0) {
        return;
    }

    particle.velocity += particle.acceleration * deltaTime;
    particle.transform[3].xyz = particle.transform[3].xyz + (particle.velocity * deltaTime);
    particle.lifetime -= deltaTime;

    if (particle.lifetime <= 0.0) {
        freelist.indices[atomicAdd(freelist.count, 1)] = index;
    }
}

layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;
void main() {
    uint index = gl_GlobalInvocationID.x;

    if (index >= particles.length())
        return;

    UpdateParticle(particles[index], int(index));
}