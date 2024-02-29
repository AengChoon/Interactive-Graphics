#version 460

in vec3 VertOutNormal;

out vec4 FragOutColor;

void main()
{
	FragOutColor = vec4(normalize(VertOutNormal), 1.0);
}