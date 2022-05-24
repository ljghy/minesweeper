#shader vertex

#version 330 core

out vec2 texCoords;
uniform int digit[3];

const vec2 coords[] = vec2[](
vec2(0, 0),
vec2(1, 1),
vec2(1, 0),
vec2(0, 0),
vec2(0, 1),
vec2(1, 1)
);

void main()
{
    int i = gl_VertexID / 6;
    vec2 coord = coords[gl_VertexID % 6];
    gl_Position = vec4((2.0 * coord.x - 1.0 + 2.0 * (i - 1.0)) / 3.0, 2.0 * coord.y - 1.0, 0.0, 1.0); 
    texCoords = vec2((coord.x + float(digit[i])) / 11.0, coord.y);
}

#shader fragment

#version 330 core
out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D tex;

void main()
{ 
    FragColor = texture(tex, texCoords);
    // FragColor = vec4(texCoords, 0.0, 1.0);
}