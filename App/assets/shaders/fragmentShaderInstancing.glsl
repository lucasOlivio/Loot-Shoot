// Fragment shader
#version 420

in vec2 vertexUV;
in float lifetime;

uniform sampler2D textureColor;

out vec4 outputColour;		// To the frame buffer (aka screen)

void main()
{
	if (lifetime <= 0.01)
		discard;

	outputColour = texture(textureColor, vertexUV);

	if (outputColour.a <= 0.1)
		discard;

	return;
}
