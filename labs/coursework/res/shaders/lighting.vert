#version 330                                                                        
                                                                                    
layout (location = 0) in vec3 position;                                                                                       
layout (location = 2) in vec3 Normal;     
layout (location = 10) in vec2 tex_coord_in;                                             
                                                                                    
uniform mat4 MVP;                                                                  
uniform mat4 gLightWVP;                                                             
uniform mat4 gWorld;                                                                
                                                                                    
out vec4 LightSpacePos;                                                             
out vec2 tex_coord_in0;                                                                 
out vec3 Normal0;                                                                   
out vec3 WorldPos0;                                                                 
                                                                                    
void main()                                                                         
{                                                                                   
    gl_position   = MVP * vec4(position, 1.0);                                  
    LightSpacePos = gLightWVP * vec4(position, 1.0);                                 
    tex_coord_in0     = tex_coord_in;                                                    
    Normal0       = (gWorld * vec4(Normal, 0.0)).xyz;                            
    WorldPos0     = (gWorld * vec4(position, 1.0)).xyz;                          
}
