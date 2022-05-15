#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;
out vec4 frag_color;

// How far (in the texture space) is the distance (on the x-axis) between
// the pixels from which the red/green (or green/blue) channels are sampled
#define STRENGTH 0.005

// Chromatic aberration mimics some old cameras where the lens disperses light
// differently based on its wavelength. In this shader, we will implement a
// cheap version of that effect 

void main(){
    //TODO: Modify this shader to apply chromatic aberration
    // To apply this effect, we only read the green channel from the correct pixel (as defined by tex_coord)
    // To get the red channel, we move by amount STRENGTH to the left then sample another pixel from which we take the red channel
    // To get the blue channel, we move by amount STRENGTH to the right then sample another pixel from which we take the blue channel

    frag_color = texture(tex, tex_coord);
    // move by amount of STRENGTH in the left direction of x is equivalent to subtracting STRENGTH
    // from only the x component while keeping the y component as it's
    // we then get the color of this new pixel and pick the red channel
    frag_color.r = texture(tex, vec2(tex_coord.x-STRENGTH, tex_coord.y)).r; 

    // move by amount of STRENGTH in the right direction of x is equivalent to adding STRENGTH
    // to only the x component while keeping the y component as it's
    // we then get the color of this new pixel and pick the blue channel
    frag_color.b = texture(tex, vec2(tex_coord.x+STRENGTH, tex_coord.y)).b;
}