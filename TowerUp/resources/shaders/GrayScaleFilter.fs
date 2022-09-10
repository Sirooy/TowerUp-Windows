#version 140

uniform sampler2D u_Texture;

void main()
{
    const float NUM_GRAYS = 20.0;

    vec4 color = texture2D(u_Texture, gl_TexCoord[0].xy);

    float avg = (color.r + color.b + color.g) / 3.0;
    avg = round(avg * NUM_GRAYS) / NUM_GRAYS;

    gl_FragColor = vec4(avg, avg, avg, 1.0);
}