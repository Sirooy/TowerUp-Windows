uniform sampler2D u_Texture;
uniform float u_Time;

void main()
{
    const float SPEED = 0.5f;
    vec4 color = texture2D(u_Texture, gl_TexCoord[0].xy);

    color.rgb = abs(color.rgb - (1.0 * (cos(u_Time * SPEED) * 0.5 + 0.5)));

    gl_FragColor = color;
}