uniform sampler2D u_Texture;

void main()
{
    vec2 coords = gl_TexCoord[0].xy;
    //coords.y = 1.0 - coords.y;
    coords.x = 1.0 - coords.x;

    gl_FragColor = texture2D(u_Texture, coords);
}