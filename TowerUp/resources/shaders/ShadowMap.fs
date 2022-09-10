#version 330

uniform int u_LightIndex;

void main()
{
    vec4 color = vec4(0.0);

    color[u_LightIndex] = 1.0;
    color.a = 1.0f;

    gl_FragColor = color; 
}