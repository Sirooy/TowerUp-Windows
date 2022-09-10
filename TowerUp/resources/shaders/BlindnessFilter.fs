#version 140

uniform sampler2D u_Texture;
uniform float u_Time;

void main()
{
    vec2 textSize = textureSize(u_Texture, 0);
    vec2 st = gl_FragCoord.xy /  textSize * 2.0 - 1.0;
    st.x *= textSize.x / textSize.y;

    vec4 color = texture2D(u_Texture, gl_TexCoord[0].xy);

    float l = (1.0 - min(1.0, length(st))) * (cos(u_Time * 5.0) * 0.5 + 0.5);
    color.rgb = color.rgb * l;

    /*
    float l = (1.0 - floor(min(1.0, length(st * 1.0 / u_Time))));
    color.rgb = color.rgb * l;
    */

    /*
    float l = (1.0 - floor(min(1.0, length(st * 1.0 / u_Time))));
    color.rgb = color.rgb * l;
    */

    gl_FragColor = color;
}