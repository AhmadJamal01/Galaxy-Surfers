#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;

out Varyings {
    vec4 color;
    vec2 tex_coord;
} vs_out;

uniform mat4 transform;

void main(){
    //DONE: (Req 6) Change the next line to apply the transformation matrix
    //* Comment: apply the transformation matrix to the position by the matrix multiplication
    //*(the position has to be in homogeneous coordinates for translation to work)
    gl_Position = transform * vec4(position, 1.0);
    vs_out.color = color;
    vs_out.tex_coord = tex_coord;
}