#version 140

uniform sampler2D u_Texture;

const float OFFSET = 1.0 / 200.0;

void main()
{
    vec2 offsets[9] = vec2[]
    (
        vec2(-OFFSET,  OFFSET), // top-left
        vec2( 0.0f,    OFFSET), // top-center
        vec2( OFFSET,  OFFSET), // top-right
        vec2(-OFFSET,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( OFFSET,  0.0f),   // center-right
        vec2(-OFFSET, -OFFSET), // bottom-left
        vec2( 0.0f,   -OFFSET), // bottom-center
        vec2( OFFSET, -OFFSET)  // bottom-right    
    );

    float kernel[9] = float[]
    (
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16  
    );

    vec2 textCoords = gl_TexCoord[0].xy;
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(u_Texture, textCoords + offsets[i]));
    }

    vec3 color = vec3(0.0);
    for(int i = 0; i < 9; i++)
        color += sampleTex[i] * kernel[i];
    color *= 0.2;

    gl_FragColor = vec4(color, 1.0);
}