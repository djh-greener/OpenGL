#shader vertex
#version 460 core
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in float inSize;

uniform mat4 view;
uniform mat4 projection;
uniform int billboard;

out VS_OUT{
    vec3 Color;
}vs_out;
void main() {
    if (billboard == 1) {
        // Billboarding: make quad always face the camera
        vec3 pos = inPosition;
        vec3 cameraRight = vec3(view[0][0], view[1][0], view[2][0]);
        vec3 cameraUp = vec3(view[0][1], view[1][1], view[2][1]);

        gl_Position = projection * view * vec4(pos, 1.0);
        gl_PointSize = inSize;
    } else {
        gl_Position = projection * view * vec4(inPosition, 1.0);
        gl_PointSize = inSize;
    }
}
#shader fragment
#version 460 core
in VS_OUT{
    vec3 Color;
}fs_in;
out vec4 FragColor;
void main(){
    FragColor = vec4(1,1,1,1.f);
}