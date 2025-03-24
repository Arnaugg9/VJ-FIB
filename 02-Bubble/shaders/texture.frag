#version 330

uniform vec4 color;
uniform sampler2D tex;
uniform float uAlpha;  // Controla la opacidad del fade-out

in vec2 texCoordFrag;
out vec4 outColor;

void main()
{
    vec4 texColor = texture(tex, texCoordFrag);
    
    if (texColor.a < 0.5)
        discard;

    // Color de la textura multiplicado por el color de entrada
    vec4 baseColor = color * texColor;
    // Aplicar el fade-out interpolando entre el color original y negro
    outColor = mix(baseColor, vec4(0.0, 0.0, 0.0, 1.0), uAlpha);
}