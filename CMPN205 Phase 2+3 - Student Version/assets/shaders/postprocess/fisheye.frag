#version 330


uniform sampler2D tex;
in vec2 tex_coord;
out vec4 frag_color;


 void main()
 {
     vec2 center = vec2(0.5f, 0.5f);
     float radius = 20.0f;
     float scale = -0.3f;

      vec2 tex_coordToUse = tex_coord;
      float dist = distance(center, tex_coord);
     tex_coordToUse -= center;
     if (dist < radius)
     {
          float percent = 1.0 + ((0.5 - dist) / 0.5) * scale;

         tex_coordToUse = tex_coordToUse * percent;
     }
     tex_coordToUse += center;

     frag_color = texture(tex, tex_coordToUse );

 }