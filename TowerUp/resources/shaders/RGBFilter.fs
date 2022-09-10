#version 140

uniform sampler2D u_Texture;
uniform float u_Time;

void main()
{
    const float SPEED = 0.5;

    vec2 FragPosition = gl_FragCoord.xy / textureSize(u_Texture, 0);

    vec4 color = texture2D(u_Texture, gl_TexCoord[0].xy);
    color.r *= cos(u_Time * SPEED + FragPosition.x) * 0.5 + 0.5;
    color.g *= cos(u_Time * SPEED + FragPosition.x + 0.7853981633) * 0.5 + 0.5;
    color.b *= sin(u_Time * SPEED + FragPosition.x) * 0.5 + 0.5;

    float sRG = sign(color.r - color.g);
    float sRB = sign(color.r - color.b);
    float sGB = sign(color.g - color.b);

    float r = max(sRG + sRB, 0.0) * color.r * 0.5;
    float g = max(sGB - sRG, 0.0) * color.g * 0.5;
    float b = max(-sGB - sRB, 0.0) * color.b * 0.5;

    gl_FragColor = vec4(r, g, b, 1.0);
}