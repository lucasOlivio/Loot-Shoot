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

struct EmitterSettings {
    vec3 minOffset, maxOffset;
    vec3 minVelocity, maxVelocity;
    vec3 minAcceleration, maxAcceleration;
    float minScale, maxScale;
    float minLife, maxLife;
    vec3 position;
};

layout(std430, binding = 0) buffer Particles {
    Particle particles[];
};

layout(std430, binding = 1) buffer FreeList {
    int count;
    int indices[];
} freelist;

uniform int particlesToSpawn;
uniform int currMS;
uniform EmitterSettings emitter;

// Random functions
uint pcg_Hash(uint seed)
{
    uint state = seed * 747796405u + 2891336453u;
    uint word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}

float rand(inout uint seed)
{
    uint x = pcg_Hash(seed);
    seed = x;
    return float(x) * uintBitsToFloat(0x2f800004u);
}

float rand1(inout uint seed, float min, float max)
{
    float randomFloat = rand(seed);

    // Scale the random float to fit the range
    return randomFloat * (max - min) + min;
}

vec3 rand3(inout uint seed, vec3 min, vec3 max)
{
    return vec3(
        rand1(seed, min.x, max.x),
        rand1(seed, min.y, max.y),
        rand1(seed, min.z, max.z)
    );
}

// make a particle with random attributes
void MakeParticle(uint seed, out Particle particle) {
    particle.lifetime = rand1(seed, emitter.minLife, emitter.maxLife);
    particle.initialLifetime = particle.lifetime;
    particle.velocity.xyz = rand3(seed, emitter.minVelocity.xyz, emitter.maxVelocity.xyz);
    particle.acceleration.xyz = rand3(seed, emitter.minAcceleration.xyz, emitter.maxAcceleration.xyz);

    float scale = rand1(seed, emitter.minScale, emitter.maxScale);
    mat4 matscale = mat4(
        scale, 0.0, 0.0, 0.0,
        0.0, scale, 0.0, 0.0,
        0.0, 0.0, scale, 0.0,
        0.0, 0.0, 0.0, 1.0
    );

    vec3 offset = rand3(seed, emitter.minOffset.xyz, emitter.maxOffset.xyz);
    vec3 position = emitter.position + offset;
    mat4 matposition = mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        position.x, position.y, position.z, 1.0
    );

    // Create rotation matrix around z-axis
    float angle = rand1(seed, 0.0, 4.0 * 3.14);
    mat4 matrotation = mat4(
        cos(angle), -sin(angle), 0.0, 0.0,
        sin(angle), cos(angle), 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );

    particle.transform = mat4(1.0);
    particle.transform = particle.transform * matposition * matrotation * matscale;
}

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;
void main() {
    uint index = gl_GlobalInvocationID.x;

    if (index >= particlesToSpawn)
        return;

    // undo decrement and return if nothing in freelist
    int freeListIndex = atomicAdd(freelist.count, -1) - 1;
    if (freeListIndex < 0) {
        atomicAdd(freelist.count, 1);
        return;
    }

    int particleIndex = freelist.indices[freeListIndex];
    uint seed = index + freelist.count + currMS;
    MakeParticle(seed, particles[particleIndex]);
}