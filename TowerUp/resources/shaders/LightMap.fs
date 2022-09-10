#version 140

struct Light
{
    vec2 Position;
    vec3 Color;
    float Radius;
    int ShadowMapIndex;
    int ShadowMapColorIndex;
};

const int MAX_LIGHTS = 21;
const int SHADOW_MAPS_PER_LIGHT = 3;

uniform Light u_Lights[MAX_LIGHTS];
uniform sampler2D u_ShadowMaps[MAX_LIGHTS / SHADOW_MAPS_PER_LIGHT];
uniform int u_ActiveLights;

void main()
{
    vec3 color = vec3(0.0);
    int currentShadowMap = 0;

    for(int i = 0;i < u_ActiveLights; ++i)
    {
        Light light = u_Lights[i];
        vec2 fragPos = gl_FragCoord.xy;
        float shadow = texture2D( u_ShadowMaps[light.ShadowMapIndex], gl_TexCoord[0].xy)[light.ShadowMapColorIndex];

        float distance = length(light.Position - fragPos);
        float ambient = smoothstep(light.Radius, 0, distance);
        color += light.Color * ambient * (1.0 - shadow);
    }

    gl_FragColor = vec4(color, 1.0);
}

//distance = light.Radius / distance;
//float ambient = (light.Radius * 50.0) / (1.0 + distance * distance * 0.44 + distance * 1.0); 