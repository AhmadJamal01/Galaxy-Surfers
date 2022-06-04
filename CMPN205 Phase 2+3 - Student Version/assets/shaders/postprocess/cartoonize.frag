#version 330

uniform sampler2D tex;
in vec2 tex_coord;
out vec4 frag_color;

float quantize(float color){
    if (color < 0.2)        return 0.1;
    else if (color < 0.4)   return 0.3;
    else if (color < 0.6)   return 0.5;
    else if (color < 0.8)   return 0.7;
    else                    return 0.9;
}

void main() {


    frag_color = texture(tex, tex_coord);

    frag_color = vec4(quantize(frag_color.r), quantize(frag_color.g), quantize(frag_color.b), 1);      // Quantize the colors
}