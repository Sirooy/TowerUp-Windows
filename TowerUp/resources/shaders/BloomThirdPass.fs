#version 140

uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;

void main()
{
    vec2 textCoords =  gl_TexCoord[0].xy;
    vec3 color1 = texture2D(u_Texture1, textCoords).rgb;
    vec3 color2 = texture2D(u_Texture2, textCoords).rgb;

    gl_FragColor = vec4(color1 + color2, 1.0);
}