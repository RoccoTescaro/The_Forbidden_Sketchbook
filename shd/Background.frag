uniform vec2 viewPortDim;

uniform vec2 resolution;
uniform sampler2D texture;

uniform vec2 viewPos;
uniform vec2 viewDim;

void main()
{
    vec2 uv = (gl_FragCoord.xy)* viewDim / viewPortDim;
    vec2 repetedUv = vec2(mod(viewPos.x+uv.x,resolution.x),mod(viewPos.y+viewDim.y-uv.y,resolution.y));
    vec4 pixelColor = texture2D(texture, repetedUv/resolution);
    gl_FragColor = pixelColor;//vec4(repetedUv/resolution,0.0,1.0);
}