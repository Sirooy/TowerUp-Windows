uniform sampler2D texture;

void main()
{
    const float AMBIENT = 0.4;
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    gl_FragColor = vec4(color.rgb * AMBIENT, color.a);
}