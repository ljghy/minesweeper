#shader vertex

#version 330 core
layout (location = 0) in vec4 aPosuv;
layout (location = 1) in float aTexId;

out vec2 texCoords;
uniform int digit[3];

void main()
{
    gl_Position = vec4(aPosuv.xy, 0.0, 1.0); 
    texCoords = vec2((aPosuv.z + float(digit[int(aTexId)])) / 11.0, aPosuv.w);
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