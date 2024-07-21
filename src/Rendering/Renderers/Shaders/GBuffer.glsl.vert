#version 450

layout(location = 0) in highp   vec3 VertPosition;
layout(location = 1) in highp   vec3 VertNormal;
layout(location = 2) in highp   vec3 VertTangent;
layout(location = 3) in mediump vec2 VertUV;

layout(std140, binding = 0) uniform u_matrices
{
    mat4 MVP;
    mat4 model_view_matrix;
    mat4 normal_matrix;
} matrices;

layout(location = 0) out highp      vec3 FPosition;
layout(location = 1) out mediump    vec2 FTextureCoord;
layout(location = 2) out highp      mat3 FTBN;

void main()
{
    gl_Position = matrices.MVP * vec4(VertPosition, 1.0f);

    highp vec3 norm   = normalize(matrices.normal_matrix * vec4(VertNormal, 1.0)).xyz;
    highp vec3 tang   = normalize(matrices.normal_matrix * vec4(VertTangent, 1.0)).xyz;
    highp vec3 binorm = cross(norm, tang);

    FTBN            = mat3(tang, binorm, norm);
    FPosition       = vec3(matrices.model_view_matrix * vec4(VertPosition, 1.0));    
    FTextureCoord   = VertUV;
}