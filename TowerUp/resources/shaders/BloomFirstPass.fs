#version 140

uniform sampler2D u_Texture;
//https://stackoverflow.com/questions/596216/formula-to-determine-perceived-brightness-of-rgb-color

void main()
{
    const float THRESHOLD = 0.5;

    vec4 color = texture2D(u_Texture, gl_TexCoord[0].xy);
    /*
    float avg = (color.r + color.g + color.b) / 3.0;
    color.rgb *= max(sign(avg - THRESHOLD), 0.0);
    */
    
    float luminance = color.r * 0.2126 + color.g * 0.7152 + color.b * 0.0722;
    color.rgb *= max(sign(luminance - THRESHOLD), 0.0);
    
    gl_FragColor = color;
}