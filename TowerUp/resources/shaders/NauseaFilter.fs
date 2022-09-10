#version 140

uniform sampler2D u_Texture;
uniform float u_Time;

void main()
{
    const float WAVE_SIZE = 0.04;
    const float WAVES = 6.0;

    vec2 st = gl_FragCoord.xy / textureSize(u_Texture, 0);
    vec2 coords = vec2(cos(u_Time         + st.y * WAVES) * WAVE_SIZE + gl_TexCoord[0].x, 
                       cos(u_Time * WAVES + st.x * WAVES) * WAVE_SIZE + gl_TexCoord[0].y);
    vec4 color = texture2D(u_Texture, coords);
    gl_FragColor = color;
}