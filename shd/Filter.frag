#version 130

uniform float threshold;
uniform vec4 previousColor;
uniform vec4 targetColor;

float r(vec2 p)
{
    return fract(cos(p.x*42.98 + p.y*43.23) * 1127.53);
}

float n(vec2 p)
{
    vec2 fn = floor(p);
    vec2 sn = smoothstep(vec2(0), vec2(1), fract(p));
    
    float h1 = mix(r(fn), r(fn + vec2(1,0)), sn.x);
    float h2 = mix(r(fn + vec2(0,1)), r(fn + vec2(1)), sn.x);
    return mix(h1 ,h2, sn.y);
}


float noise(vec2 p)
{
    return n(p/32.) * 0.58 +
           n(p/16.) * 0.2  +
           n(p/8.)  * 0.1  +
           n(p/4.)  * 0.05 +
           n(p/2.)  * 0.02 +
           n(p)     * 0.0125;
}


void main()
{
    float alpha = noise(gl_FragCoord.xy*0.025f);
    if(alpha > threshold)
        gl_FragColor = previousColor;
    else
        gl_FragColor = targetColor;
}

