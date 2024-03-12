#version 460

in vec3 VertOutViewNormal;
in vec4 VertOutObjectViewPosition;

uniform vec3 LightViewPosition;

vec3 DiffuseColor = vec3(0.5, 0.2, 0.7);
vec3 AmbientColor = vec3(0.2, 0.2, 0.2);
vec3 SpecularIntensity = vec3(0.7);
float SpecularPower = 50.0;

out vec4 FragOutColor;

void main()
{
	vec3 ViewNormal = normalize(VertOutViewNormal);

	vec3 VectorToLight = LightViewPosition.xyz - VertOutObjectViewPosition.xyz;
	vec3 DirectionToLight = normalize(VectorToLight);
	vec3 DirectionToCamera = normalize(-VertOutObjectViewPosition.xyz);
	vec3 HalfDirection = normalize(DirectionToLight + DirectionToCamera);

	vec3 Diffuse = max(dot(ViewNormal, DirectionToLight), 0.0) * DiffuseColor;
	vec3 Specular = pow(max(dot(HalfDirection, ViewNormal), 0.0), SpecularPower) * SpecularIntensity;

	FragOutColor = vec4((AmbientColor + Diffuse) * vec3(1.0, 0.0, 0.0) + Specular, 1.0);
}