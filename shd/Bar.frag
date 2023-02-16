#version 130

//loading direction
uniform sampler2D texture;
uniform vec2 start;
uniform vec2 end;
uniform float value; // 0-1

//color mask
uniform vec3 colorMask;
uniform float threshold = 0.55;

//texture mask?
uniform sampler2D textureMask;

uniform float windowHeight;

void main()
{
    vec4 pixelColor = texture2D(texture, gl_TexCoord[0].xy);
    float pixelAlpha = texture2D(textureMask, gl_TexCoord[0].xy).a;
    vec2 pixelCoord = vec2(gl_FragCoord.x,windowHeight-gl_FragCoord.y);

    //To visualize it: https://www.geogebra.org/m/jujyfsdz
    vec2 u = end - start;
    vec2 charge = start + u*value;
    vec2 w = pixelCoord - charge;

    float a = dot(u,w);

    if(pixelAlpha > 0 && distance(pixelColor.rgb, colorMask) < threshold)
        if(a > 0)
            pixelColor.a *= 0.15;

    gl_FragColor = pixelColor;
}