//VERTEX SHADER
#version 450

// normally stored in vertex buffer
vec2 positions[3] = vec2[]( 
    vec2(0.0f, -0.5), 
    vec2(0.5, 0.5), 
    vec2(-0.5, 0.5) 
);
// per-vertex colors
vec3 colors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

layout(location = 0) out vec3 vertexColor;

void main() {
    // gl_VertexIndex contains index of the current vertex (usually an index into the vertex buffer)
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0); // dummy z & w components
    vertexColor = colors[gl_VertexIndex];
}