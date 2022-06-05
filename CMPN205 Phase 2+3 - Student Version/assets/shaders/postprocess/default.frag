#version 330


uniform sampler2D tex;
in vec2 tex_coord;
out vec4 frag_color;


 void main()
 {
     frag_color = texture(tex, tex_coord );
 }
 


