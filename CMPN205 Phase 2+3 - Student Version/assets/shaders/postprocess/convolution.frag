#version 330

uniform sampler2D tex;
in vec2 tex_coord;
out vec4 frag_color;

void main() {

    ivec2 texture_size = textureSize(tex, 0);       // returns ivec2 of the texture's dimensions
    vec2 step_size = 1.0/texture_size;              // The step between each two pixels in the texture


    vec2 neighborhood[9] =  vec2[](            // The textured pixel and its 8-neighborhood as offsets from it
        tex_coord + vec2(-step_size.x, -step_size.y), tex_coord + vec2( 0.0f, -step_size.y),  tex_coord + vec2( step_size.x, -step_size.y),
        tex_coord + vec2(-step_size.x, 0.0f),         tex_coord + vec2( 0.0f, 0.0f),          tex_coord + vec2( step_size.x,   0.0f),
        tex_coord + vec2(-step_size.x, +step_size.y), tex_coord + vec2( 0.0f, +step_size.y),  tex_coord + vec2( step_size.x, step_size.y)
    );

    float kernel[9] = float[](              // Sharpening filter with some dimming (-0.3)
        0, -1, 0,
        -1,  7, -1,
        0, -1, -0.3
    );


    vec4 sum = vec4(0.0);
    for(int i = 0; i < 9; i++)          
        sum += texture(tex, neighborhood[i]) * kernel[i];   // Apply Convolution

    sum.a = 1.0;
    frag_color = sum;

    frag_color = vec4(frag_color.r,frag_color.g, frag_color.b, 1);      

    frag_color = vec4( frag_color.r,frag_color.g, frag_color.b, 1);        // Would you like an inverted red channel?


}