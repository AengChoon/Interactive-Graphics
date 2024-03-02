#version 460

in vec3 VertOutColor;

out vec4 FragOutColor;

void main()
{
	FragOutColor = vec4(VertOutColor, 1.0);
}