#version 440

// Calculates the shadow factor of a vertex
float calculate_shadow(in sampler2D shadow_map, in vec4 light_space_pos) {
    vec3 proj_coords = light_space_pos.xyz / light_space_pos.w;
    vec2 shadow_tex_coords;
    shadow_tex_coords.x = 0.5 * proj_coords.x + 0.5;
    shadow_tex_coords.y = 0.5 * proj_coords.y + 0.5;
    if (light_space_pos.z < 0 || shadow_tex_coords.x < 0 || shadow_tex_coords.x > 1 || shadow_tex_coords.y < 0 || shadow_tex_coords.y > 1){
        return 1.0;
    }

    float depth = (texture(shadow_map, shadow_tex_coords)).z;
    float range = (
		light_space_pos.x * light_space_pos.x +
		light_space_pos.y * light_space_pos.y +
		light_space_pos.z * light_space_pos.z
	);
	range /= 1000000.0;
    if (depth == 0.0 || depth == 1.0) {
        return 1.0;
    }
	else if (depth < range - 0.003){
        return depth;
    }
	 else {
        return 1.0f;
    }

}