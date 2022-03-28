#version 330 core

out vec4 frag_color;

//DONE: Define uniforms for the center and the side-length
uniform vec2 center;                                        // The diamond's center
uniform float side_length;                                  // The diamond's side length (vertical/horizonal axes)

uniform vec4 inside_color = vec4(1.0, 0.0, 0.0, 1.0);
uniform vec4 outside_color = vec4(0.0, 0.0, 0.0, 1.0);

void main(){
    //DONE: Write code that will draw the diamond
    float distance = abs(gl_FragCoord.x - center.x) + abs(gl_FragCoord.y - center.y);   //Manhattan Distance

    if(distance <= side_length/2.0){        //diamond equn is abs(x) + abs(y) <= r (where L=W=2r)
        frag_color = inside_color;
    } else {
        frag_color = outside_color;
    }

   
}