#version 140

uniform sampler2D u_LightMap;
uniform sampler2D u_Texture;

void main()
{
    const float ambient = 0.2;
    vec2 textCoords =  gl_TexCoord[0].xy;
    
    vec3 light = texture2D(u_LightMap, textCoords).rgb;
    vec3 color = texture2D(u_Texture, textCoords).rgb;

    gl_FragColor = vec4(color * ambient + color * light, 1.0);
}