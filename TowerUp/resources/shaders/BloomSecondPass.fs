#version 140

uniform sampler2D u_Texture;
uniform bool u_HorizontalBlur;

void main()
{
    const float weights[5] = float[] ( 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216 );
    //const float weights[3] = float[] ( 0.274069,0.451863,0.274069 );
    vec2 textSize = 1.0 / textureSize(u_Texture, 0);
    vec2 textCoords = gl_TexCoord[0].xy;

    vec3 color = texture(u_Texture, textCoords).rgb * weights[0];

    if(u_HorizontalBlur)
    {
        for(int i = 1;i < 5; ++i)
        {
            color.rgb += texture(u_Texture, textCoords + vec2(textSize.x * i, 0.0)).rgb * weights[i];
            color.rgb += texture(u_Texture, textCoords - vec2(textSize.x * i, 0.0)).rgb * weights[i];
        }
    }
    else
    {
        for(int i = 1;i < 5; ++i)
        {
            color.rgb += texture(u_Texture, textCoords + vec2(0.0, textSize.y * i)).rgb * weights[i];
            color.rgb += texture(u_Texture, textCoords - vec2(0.0, textSize.y * i)).rgb * weights[i];
        }
    }

    gl_FragColor = vec4(color, 1.0);
}