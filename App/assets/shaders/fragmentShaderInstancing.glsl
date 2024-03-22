// Fragment shader
#version 420

in vec4 vertexColor;
in vec2 vertexUV;
in float lifetime;

uniform sampler2D textureColor;

out vec4 outputColour;		// To the frame buffer (aka screen)

void main()
{
	if (lifetime <= 0.01)
		discard;

	outputColour = (texture(textureColor, vertexUV) * vertexColor);

	if (outputColour.a <= 0.15)
		discard;

	return;
}
