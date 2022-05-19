#shader vertex

#version 330 core
layout (location = 0) in vec4 aPosuv;

uniform vec4 view;

out vec2 texCoords;

void main()
{
    gl_Position = vec4(aPosuv.xy, 0.0, 1.0); 
    texCoords = aPosuv.zw * view.xy + view.zw;
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