#shader vertex

#version 330 core
layout (location = 0) in vec4 aPosuv;
layout (location = 1) in float aTexId;

uniform vec4 view;

out vec2 texCoords;
out float texId;

void main()
{
    gl_Position = vec4(aPosuv.xy * view.xy + view.zw, 0.0, 1.0); 
    texCoords = aPosuv.zw;
    texId = aTexId;
}

#shader fragment

#version 330 core
out vec4 FragColor;

in vec2 texCoords;
in float texId;

uniform sampler2D tex[14];

void main()
{ 
    FragColor = texture(tex[int(texId)], texCoords);
}