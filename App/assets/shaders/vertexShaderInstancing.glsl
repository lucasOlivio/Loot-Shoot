// Vertex shader
#version 420

//uniform mat4 MVP;
uniform mat4 matView;
uniform mat4 matProjection;

// Particle variables
layout (location = 0) in vec4 vPos;
layout (location = 1) in vec4 instanceColor;
layout (location = 2) in float instanceAlpha;
layout (location = 3) in float instanceLifeTime;
layout (location = 4) in mat4 instanceTransform;

// Fragment shader variables
out vec4 vertexColor;
out float lifetime;

void main()
{
	vec4 vertexModelPosition = vPos;

	mat4 matMVP = matProjection * matView * instanceTransform;
	gl_Position = matMVP * vec4(vertexModelPosition.xyz, 1.0);

	vertexColor = instanceColor * instanceAlpha;
	lifetime = instanceLifeTime;
}

