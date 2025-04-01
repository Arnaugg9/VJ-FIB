#version 330

uniform vec4 color;
uniform sampler2D tex;

in vec2 texCoordFrag;
out vec4 outColor;

void main()
{
    vec4 texColor = texture(tex, texCoordFrag);
    
    if (texColor.a < 0.5)
        discard;

    // Color de la textura multiplicado por el color de entrada
    outColor = color * texColor;
}