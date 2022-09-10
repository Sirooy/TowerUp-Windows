uniform sampler2D u_Texture;

void main()
{
    const float FACTOR = 250.0;

    vec2 coords = gl_TexCoord[0].xy;
    coords.x = floor(coords.x * FACTOR) / FACTOR;
    coords.y = floor(coords.y * FACTOR) / FACTOR;

    gl_FragColor = texture2D(u_Texture, coords);
}