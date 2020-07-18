#version 450

layout(location = 0) in vec3 VertPosition;
layout(location = 1) in vec3 VertNormal;
layout(location = 2) in vec3 VertTangent;
layout(location = 3) in vec2 VertUV;

layout(std140, binding = 0) uniform u_matrices
{
    mat4 MVP;
    mat3 model_matrix;
} matrices;

layout(location = 0) out vec3 FPosition;
layout(location = 1) out vec2 FTextureCoord;
layout(location = 2) out mat3 FTBN;

void main()
{
    gl_Position = matrices.MVP * vec4(VertPosition, 1.0f);

    vec3 norm   = normalize(matrices.model_matrix * VertNormal);
    vec3 tang   = normalize(matrices.model_matrix * VertTangent);
    vec3 binorm = cross(norm, tang);

    FTBN            = mat3(tang, binorm, norm);
    FPosition       = matrices.model_matrix * VertPosition;    
    FTextureCoord   = VertUV;
}
