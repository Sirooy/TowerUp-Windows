#version 140

uniform sampler2D texture;

float WhenGreater(float n1, float n2)
{
    return max(sign(n1 - n2), 0.0);
}

float WhenLessOrEqual(float n1, float n2)
{
    return 1.0 - max(sign(n1 - n2), 0.0);
}

void main()
{
    const float BORDER_SIZE = 2.0;

    vec2 texelSize = BORDER_SIZE / textureSize(texture, 0);
    vec2 texCoords = gl_TexCoord[0].xy;
    vec4 color = gl_Color * texture2D(texture, texCoords);

    float alpha =  texture2D(texture, texCoords + vec2(-texelSize.x, -texelSize.y)).a;
    alpha +=       texture2D(texture, texCoords + vec2( 0.0        , -texelSize.y)).a;
    alpha +=       texture2D(texture, texCoords + vec2( texelSize.x, -texelSize.y)).a;
    alpha +=       texture2D(texture, texCoords + vec2(-texelSize.x, 0.0         )).a;
    alpha +=       texture2D(texture, texCoords + vec2( texelSize.x, 0.0         )).a;
    alpha +=       texture2D(texture, texCoords + vec2(-texelSize.x,  texelSize.y)).a;
    alpha +=       texture2D(texture, texCoords + vec2( 0.0        ,  texelSize.y)).a;
    alpha +=       texture2D(texture, texCoords + vec2( texelSize.x,  texelSize.y)).a;

    color += WhenLessOrEqual(color.a, 0.0) * WhenGreater(alpha, 0.0) * vec4(1.0);
    
    gl_FragColor = color;
}