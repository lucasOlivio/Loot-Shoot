// Fragment shader
#version 460 core

in vec2 vertexUV;
in float lifetime;
in float initialLifetime;

uniform sampler2D textureColor;

uniform int rowsTexture;
uniform int colsTexture;

out vec4 outputColour;		// To the frame buffer (aka screen)

void main()
{
    if (lifetime <= 0.01)
        discard;

    vec2 cellSize = vec2(1.0 / colsTexture, 1.0 / rowsTexture);
    vec2 uv = vec2(vertexUV.x / float(colsTexture), vertexUV.y / float(rowsTexture)); // Normalize UV coordinates

    int numFrames = colsTexture * rowsTexture;
    float ratioLife = (initialLifetime - lifetime) / initialLifetime;
    int currFrame = int(ratioLife * numFrames);

    vec2 offset = vec2(0.0);
    offset.x = cellSize.x * (currFrame % colsTexture);
    offset.y = cellSize.y * int(currFrame / rowsTexture);

    // Blend the vertex UV with the texture atlas UV position based on the lifetime
    vec2 finalUV = uv + offset;

    // Sample the texture using the final UV coordinates
    outputColour = texture(textureColor, finalUV);

    if (outputColour.a <= 0.1)
        discard;

    return;
}
