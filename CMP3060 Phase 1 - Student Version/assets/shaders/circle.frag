#version 330 core

out vec4 frag_color;

//DONE: Define uniforms for the center and the radius
uniform vec2 center;
uniform float radius;

uniform vec4 inside_color = vec4(1.0, 0.0, 0.0, 1.0);
uniform vec4 outside_color = vec4(0.0, 0.0, 0.0, 1.0);

void main(){
    //DONE: Write code that will draw the circle
    float distance = (gl_FragCoord.x - center.x) * (gl_FragCoord.x - center.x) + (gl_FragCoord.y - center.y) * (gl_FragCoord.y - center.y);
    float radius_sq = radius * radius;

    if(distance <= radius_sq){
        frag_color = inside_color;
    } else {
        frag_color = outside_color;
    }
}