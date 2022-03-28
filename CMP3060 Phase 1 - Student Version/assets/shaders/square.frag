#version 330 core
// previous line specifies the glsl version to be used
// 'core' means don't allow usage of any deprecated functions => stable-branch instead of latest :)

// this fragment shader will be called on each pixel on rendering
// so to draw a square, we simply check if the pixel that's currently being processed
// is within the square region we set frag_color to inside_color which is the color of the square
// instead we set it to outside_color 
// fragment color should output a color to be applied on the pixel, this is done with the help
// of the next line :)
out vec4 frag_color;

//TODO: Define uniforms for the center and the side-length
uniform vec2 center;                
uniform float side_length;
uniform vec4 inside_color = vec4(1.0, 0.0, 0.0, 1.0);
uniform vec4 outside_color = vec4(0.0, 0.0, 0.0, 1.0);

void main(){

    //TODO: Write code that will draw the square
    // gl_FragCoord contains the window-relative coordinates of the current fragment
    // only accessible inside fragment 
    vec2 norm_distance = gl_FragCoord.xy - center;    
    
    if(max(abs(norm_distance[0]), abs(norm_distance[1])) <= side_length/2.0){
        frag_color = inside_color;
    } else {
        frag_color = outside_color;
    }
}