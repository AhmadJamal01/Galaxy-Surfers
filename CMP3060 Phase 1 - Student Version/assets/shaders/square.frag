#version 330 core

out vec4 frag_color;

//TODO: Define uniforms for the center and the side-length
uniform vec2 center;
uniform float side_length;
uniform vec4 inside_color = vec4(1.0, 0.0, 0.0, 1.0);
uniform vec4 outside_color = vec4(0.0, 0.0, 0.0, 1.0);

void main(){

    //TODO: Write code that will draw the square
    
    vec2 norm_distance = gl_FragCoord.xy - center;    
    
    if(max((norm_distance[0]), (norm_distance[1])) <= side_length/2.0){
        frag_color = inside_color;
    } else {
        frag_color = outside_color;
    }
}