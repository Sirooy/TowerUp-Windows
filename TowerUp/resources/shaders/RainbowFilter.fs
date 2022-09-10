#version 140

uniform sampler2D u_Texture;
uniform float u_Time;

void main()
{
    const float SPEED = 8.0;

    vec2 FragPosition = gl_FragCoord.xy / textureSize(u_Texture, 0);

    vec4 color = texture2D(u_Texture, gl_TexCoord[0].xy);
    color.r *= cos(u_Time * SPEED + FragPosition.x) * 0.5 + 0.5;
    color.g *= cos(u_Time * SPEED + FragPosition.x + 0.7853981633) * 0.5 + 0.5;
    color.b *= sin(u_Time * SPEED + FragPosition.x) * 0.5 + 0.5;

    gl_FragColor = color;
}