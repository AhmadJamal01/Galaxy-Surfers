#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;

out vec4 frag_color;

// Vignette is a postprocessing effect that darkens the corners of the screen
// to grab the attention of the viewer towards the center of the screen

void main(){
    //TODO: Modify this shader to apply vignette
    // To apply vignette, divide the scene color
    // by 1 + the squared length of the 2D pixel location the NDC space
    // Hint: remember that the NDC space ranges from -1 to 1
    // while the texture coordinate space ranges from 0 to 1
    // We have the pixel's texture coordinate, how can we compute its location in the NDC space?
    
    // pixel coordinate range is computed from ndc space by:
    //  - divide by 2
    //  - add 0.5
    // to reverse this, we subtract 0.5 and then multiply by 2
    vec2 ndc_coord = (tex_coord - 0.5) * 2.0;

    frag_color = texture(tex, tex_coord);

    // calculate the length of the vector with the help of 'length' function 
    frag_color = frag_color / (1 + length(ndc_coord)*length(ndc_coord));
}