#version 460

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;

uniform mat4 ModelViewMatrix;
uniform mat4 ModelViewProjectionMatrix;
uniform mat4 LightViewMatrix;
uniform vec3 LightPosition;

vec3 DiffuseColor = vec3(0.5, 0.2, 0.7);
vec3 AmbientColor = vec3(0.2, 0.2, 0.2);

out vec3 VertOutColor;

void main()
{
	vec4 ObjectViewPosition = ModelViewMatrix * vec4(Position, 1.0);
	vec4 LightViewPosition = LightViewMatrix * vec4(LightPosition, 1.0);
	vec3 ViewNormal = normalize(mat3(ModelViewMatrix) * Normal);

	vec3 VectorToLight = LightViewPosition.xyz - ObjectViewPosition.xyz;
	vec3 DirectionToLight = normalize(VectorToLight);

	vec3 Diffuse = max(dot(ViewNormal, DirectionToLight), 0.0) * DiffuseColor;

	VertOutColor = (AmbientColor + Diffuse) * vec3(1.0, 0.0, 0.0);
    gl_Position = ModelViewProjectionMatrix * vec4(Position, 1.0);
}