#version 440                                                                        
                                                                                    
// Incoming position
layout (location = 0) in vec3 position;
// Incoming normal
layout (location = 2) in vec3 normal;
// Incoming texture coordinate
layout (location = 10) in vec2 tex_coord_in;                                               
                                                                                    
uniform mat4 MVP;                                                                  
                                                                                    
// Outgoing texture coordinate
layout (location = 2) out vec2 tex_coord_out;                                                          
                                                                                    
void main()                                                                         
{                                                                                   
    gl_Position = MVP * vec4(position, 1.0);                                   
    tex_coord_out = tex_coord_in;                                                       
}
