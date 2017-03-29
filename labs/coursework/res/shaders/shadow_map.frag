#version 440                                                                        
                                                                                    
// Incoming texture coordinate
layout(location = 2) in vec2 tex_coord;
                                                             
// Shadow map to sample from
uniform sampler2D shadow_map;                                                       
                                                                                    
// Outgoing colour
layout(location = 0) out vec4 colour;                                                                
                                                                                    
void main()                                                                         
{                                                                                   
    float depth = texture(shadow_map, tex_coord).x;                               
    depth = 1.0 - (1.0 - depth) * 25.0;                                             
    colour = vec4(depth);                                                        
}
