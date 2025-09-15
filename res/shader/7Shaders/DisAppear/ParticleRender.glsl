#shader vertex
#version 460 core
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in float inSize;

out VS_OUT {
    vec3 Color;
    float Size;
} vs_out;

void main() {
    gl_Position = vec4(inPosition, 1.0);
    vs_out.Color = inColor.rgb;
    vs_out.Size = inSize;
}

#shader geometry
#version 460 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    vec3 Color;
    float Size;
} gs_in[];

out GS_OUT {
    vec3 Color;
    vec2 TexCoord;
    vec3 Normal;
} gs_out;

uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 centerPos = gl_in[0].gl_Position;
    float size = gs_in[0].Size;
    
    // Get the camera's forward direction (negative Z-axis in view space)
    vec3 cameraForward = normalize(vec3(view[0][2], view[1][2], view[2][2]));
    
    // Generate quad vertices facing the camera
    vec4 v1 = centerPos + vec4(-size, size, 0.0, 0.0);
    vec4 v2 = centerPos + vec4(size, size, 0.0, 0.0);
    vec4 v3 = centerPos + vec4(-size, -size, 0.0, 0.0);
    vec4 v4 = centerPos + vec4(size, -size, 0.0, 0.0);

    // Rotate vertices to face the camera
    v1.xyz = centerPos.xyz + (v1.xyz - centerPos.xyz) * mat3(view);
    v2.xyz = centerPos.xyz + (v2.xyz - centerPos.xyz) * mat3(view);
    v3.xyz = centerPos.xyz + (v3.xyz - centerPos.xyz) * mat3(view);
    v4.xyz = centerPos.xyz + (v4.xyz - centerPos.xyz) * mat3(view);

    v1 = projection * view * v1;
    v2 = projection * view * v2;
    v3 = projection * view * v3;
    v4 = projection * view * v4;

    // Emit vertices with texture coordinates and normals
    gs_out.Normal = cameraForward;
    gs_out.Color = gs_in[0].Color;
    // Triangle 1
    gs_out.TexCoord = vec2(0.0, 1.0);
    gl_Position = v1;
    EmitVertex();
    
    gs_out.TexCoord = vec2(1.0, 1.0);
    gl_Position = v2;
    EmitVertex();
    
    gs_out.TexCoord = vec2(0.0, 0.0);
    gl_Position = v3;
    EmitVertex();
    
    gs_out.TexCoord = vec2(1.0, 0.0);
    gl_Position = v4;
    EmitVertex();
    
    EndPrimitive();
}

#shader fragment
#version 460 core
in GS_OUT {
    vec3 Color;
    vec2 TexCoord;
    vec3 Normal;
} fs_in;

uniform sampler2D tex;
out vec4 FragColor;

void main() {
    FragColor =  vec4(fs_in.Color, texture(tex, fs_in.TexCoord).x);//);

    //FragColor = vec4(1,1,0, 1.0);
}