#version 330 core

in Varyings {
    vec3 position;
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
} fs_in;

out vec4 frag_color;

uniform sampler2D tex;

void main(){
    //TODO: (Req 4) Change the following line to read the fragment color
    // from the texture at the received texture coordinates
    frag_color = texture(tex, fs_in.tex_coord);
    //@fun texture: Retrieves texels (texture pixels) from a texture
    //@param tex: gsampler1D sampler. The uniform sample attached to the texture unit corresponding to the texture
    //@param fs_in.tex_coord: specifies the texture coordinates at which texture will be sampled.
    // Assuming that each vertex in the triangle mesh is associated with a texture pixel
    // all vertices forming the triangle will be colored by sampling the texture (via by interpolation.)
}

