#shader vertex

#version 330 core
layout (location = 0) in float aTexId;

uniform vec4 view;
uniform int width;

const vec2 coords[] = vec2[](
vec2(0, 0),
vec2(1, 1),
vec2(1, 0),
vec2(0, 0),
vec2(0, 1),
vec2(1, 1)
);

out vec2 texCoords;

void main()
{
    int i = gl_VertexID / 6;
    int r = i / width, c = i % width;
    vec2 coord = coords[gl_VertexID % 6];
    gl_Position = vec4((coord + vec2(c, r)) * view.xy + view.zw, 0.0, 1.0); 
    texCoords = vec2(coord.x, (coord.y + aTexId) / 15.0);
}

#shader fragment

#version 330 core
out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D tex;
uniform float opacity;
void main()
{ 
    vec4 texColor = texture(tex, texCoords);
    FragColor = vec4(texColor.xyz, mix(texColor.w, 1.0, opacity));
}