#version 460

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;

uniform mat4 ModelViewMatrix;
uniform mat4 ModelViewProjectionMatrix;

out vec3 VertOutNormal;

void main()
{
	VertOutNormal = mat3(transpose(inverse(ModelViewMatrix))) * Normal;
    gl_Position = ModelViewProjectionMatrix * vec4(Position, 1.0);
}