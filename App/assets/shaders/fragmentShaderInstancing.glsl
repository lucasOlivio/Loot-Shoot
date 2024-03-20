// Fragment shader
#version 420

in vec4 vertexColor;
in float lifetime;

out vec4 outputColour;		// To the frame buffer (aka screen)

void main()
{
	if (lifetime <= 0.01)
		discard;

	outputColour = vertexColor;
	return;
}
