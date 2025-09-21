#version 120

uniform sampler2D texture;
uniform int levels;

void main() {
    vec4 tex = texture2D(texture, gl_TexCoord[0].xy);

    float steps = floor(pow(float(levels), 1.0/3.0) + 0.5);

    float r = floor(tex.r * steps) / (steps - 1.0);
    float g = floor(tex.g * steps) / (steps - 1.0);
    float b = floor(tex.b * steps) / (steps - 1.0);

    gl_FragColor = vec4(r, g, b, tex.a);
}