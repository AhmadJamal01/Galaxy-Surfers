#version 330 core

out vec4 frag_color;

//TODO: Define uniforms for the slope and the intercept

//uniforms are global, they will keep their values until they're either reset or updated using glUniform__()
uniform float slope = 1.0;
uniform float intercept = 0.0;

uniform vec4 inside_color = vec4(1.0, 0.0, 0.0, 1.0);
uniform vec4 outside_color = vec4(0.0, 0.0, 0.0, 1.0);

void main(){
    //TODO: Write code that will draw the line
    // -->
    // gl_FragCoord — contains the window-relative coordinates of the current fragment
    // by default, gl_FragCoord assumes a lower-left origin for window coordinates
    // using these coordiates and the equation of the required straight line
    // we can plug the coordinates into the straight line and check if the given point lies above
    // or below the line, and accordingly give it the desired color
    if(gl_FragCoord.y <= slope*gl_FragCoord.x+intercept){
        frag_color = inside_color;
    } else {
        frag_color = outside_color;
    }
}