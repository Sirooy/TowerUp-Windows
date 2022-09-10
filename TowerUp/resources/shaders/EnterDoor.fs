#version 140

uniform sampler2D u_Texture;
uniform float u_Time;
uniform vec2 u_Position;

void main()
{
    const float MAX_RADIUS = 3.0;
    const float DURATION = 1.0;
    const float SMOOTHNESS = 0.1;
    
    vec2 textSize = textureSize(u_Texture, 0);
    vec2 st = gl_FragCoord.xy /  textSize;
    vec2 center = st * 2.0 - 1.0 + u_Position;
    center.x *= textSize.x / textSize.y;

    vec4 color = texture2D(u_Texture, gl_TexCoord[0].xy);
    
    float r = u_Time / (DURATION / MAX_RADIUS);
    float mult = smoothstep(r, r + SMOOTHNESS, MAX_RADIUS - length(center));

    gl_FragColor = color * mult;
}